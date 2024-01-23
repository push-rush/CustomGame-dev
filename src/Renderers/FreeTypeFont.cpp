#include "../../include/Renderers/FreeTypeFont.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Game.h"

FreeTypeFont::FreeTypeFont(const std::string& path)
{
    this->mGame = nullptr;
    this->mFontFace = nullptr;

    this->load(path);
}

FreeTypeFont::FreeTypeFont(class Game* game)
{
    this->mGame = game;
    this->mFontFace = nullptr;
}

FreeTypeFont::~FreeTypeFont()
{
    FT_Done_Face(*this->mFontFace);
    FT_Done_FreeType(this->mFT_Library);
}

void FreeTypeFont::load(const std::string& fontFile)
{
    this->mFontFile = fontFile;

    // 初始化FreeType库
    if (FT_Init_FreeType(&mFT_Library))
    {
        SDL_Log("[FreeTypeFont] Failed to init font...");
    }

    // 加载字体
    this->mFontFace = new FT_Face();
    if (FT_New_Face(this->mFT_Library, fontFile.c_str(), 0, this->mFontFace))
    {
        SDL_Log("[FreeTypeFont] Failed to load font...");
        return;
    }

    // 定义字体大小
    // 宽度值设为0会从字体面根据给定的高度动态计算字形的宽度
    FT_Set_Pixel_Sizes(*this->mFontFace, 0, 30);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
    for (wchar_t c = 0; c < 128; ++c)
    {
        loadWChar(c);
    }
}

void FreeTypeFont::unLoad()
{

}

Vector2 FreeTypeFont::renderText(std::vector<Texture*>& textures, const wchar_t* text, Vector3 color, int pointSize)
{
    Vector2 offset = Vector2{0.0f, 0.0f};
    float width_sum = 0.0f;
    int len_w = wcslen(text);
    
    for (int i = 0; i < len_w; ++i)
    {
        // std::cout << "[FreeTypeFont] Unicode: " << text[i] << std::endl;

        auto iter = this->mCharacters.find(text[i]);
        Character* ch = nullptr;
        if (iter != this->mCharacters.end())
        {
            ch = (*iter).second;
        }

        if (!ch || ch->Height != static_cast<GLubyte>(pointSize))
        {
            ch = this->loadWChar(text[i], pointSize);
        }
        else
        {
            bool exists = false;
            for (auto tex : textures)
            {
                if (tex->getTextureID() == ch->TextureID)
                {
                    exists = true;
                    break;
                }
            }

            if (exists)
            {
                width_sum += ch->Size.y;
                offset.x += (ch->Advance >> 6);
                continue;
            }
        }
        
        // std::cout << "h:" << ch->Size.y << std::endl;

        Texture* tex = this->getFontTexture(ch, offset);
        textures.emplace_back(tex);

        // 更新位置到下一个字形的原点，注意单位是1/64像素
        width_sum += ch->Size.y;
        offset.x += (ch->Advance >> 6); // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
        // std::cout << "text_length:" << text_length << " x:" << offset.x << std::endl;
        // std::cout << "x_offset:" << (ch.Advance >> 6) << " advance:" << (ch.Advance / 64) << std::endl;
    }

    // std::cout << "width_sum:" << width_sum / text.size() << std::endl;
    return Vector2{offset.x, width_sum / len_w};
}

Character* FreeTypeFont::renderFont(const wchar_t& wch, const Vector3& color, const int& size)
{
    Character* ch = nullptr;
    auto iter = this->mCharacters.find(wch);
    if (iter != this->mCharacters.end())
    {
        ch = (*iter).second;
    }

    if (!ch || ch->Height != static_cast<GLubyte>(size))
    {
        ch = this->loadWChar(wch, size);
    }

    return ch;
}

Character* FreeTypeFont::loadWChar(const wchar_t& wch, const GLubyte& fontSize)
{
    // 设置字体大小
    FT_Set_Pixel_Sizes(*this->mFontFace, 0, fontSize);

    // 加载字符的字形 
    if (FT_Load_Char((*this->mFontFace), wch, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return nullptr;
    }

    // 生成纹理
    GLuint texture_id;
    FreeTypeFont::createFromSurface(this->mFontFace, texture_id);

    // 储存字符供之后使用
    Character* ch = new Character{
        texture_id, 
        glm::ivec2((*this->mFontFace)->glyph->bitmap.width, (*this->mFontFace)->glyph->bitmap.rows),
        glm::ivec2((*this->mFontFace)->glyph->bitmap_left, (*this->mFontFace)->glyph->bitmap_top),
        (*this->mFontFace)->glyph->advance.x,
        fontSize
    };

    // 存储字符供后续使用
    this->mCharacters.insert({wch, ch});

    return ch;
}

void FreeTypeFont::createFromSurface(FT_Face* surface, GLuint& texture_id)
{
     if ((*surface)->glyph)
    {
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            (*surface)->glyph->bitmap.width,
            (*surface)->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            (*surface)->glyph->bitmap.buffer
        );

        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        SDL_Log("[Texture] Create surface texture failed...");
    }
}

class Texture* FreeTypeFont::getFontTexture(class Character* ch, const Vector2& offset)
{
    GLfloat pos_x = offset.x + ch->Bearing.x;
    GLfloat pos_y = offset.y - (ch->Size.y - ch->Bearing.y);

    GLfloat w = ch->Size.x;
    GLfloat h = ch->Size.y;

    Texture* tex = new Texture();

    tex->setWidth(w);
    tex->setHeight(h);
    tex->setPosOffset(Vector2{pos_x, pos_y});
    tex->setTextureID(ch->TextureID);

    return tex;
}