#include "../../include/Renderers/TextButton.h"

#include "../../include/Renderers/UIScreen.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/Graphics2d.h"
#include "../../include/Renderers/FreeTypeFont.h"

#include "../../include/Game.h"

#include <SDL2/SDL.h>

TextButton::TextButton(
    class UIScreen* ui,
    const std::string& name, const std::string& type,
    const Vector2& pos, const Vector2& dims, const Vector2& scale, 
    const Vector3& bgColor, const Vector3& boxColor,
    const std::wstring& text, const Vector3& color, const int& fontSize, const Vector2& bindTextOffset,
    const std::map<std::string, std::string>& bindTexName, const Vector2& bindTexOffset,
    const int& bind_event,
    std::function<void()> onClick,
    const bool& addIn
) : Button(ui, name, type, pos, dims, scale, bgColor, boxColor, text, color, fontSize, bindTextOffset, bindTexName, bindTexOffset, bind_event, onClick, addIn)
{
    this->mModText = text;
    this->mMaxTextLength = 50;
    this->mTextBoxColor = Vector3{0.75f, 0.75f, 0.75f};
    this->mTextBGColor = Vector3{0.25f, 0.25f, 0.25f};
    
    for (auto tex : this->getNameTextures())
    {
        this->mModTextTextures.emplace_back(tex);
    }
    this->mModTextSize = this->getTextSize();

    this->mCurTimestamp = SDL_GetTicks();

    this->mLinkButtonPos = pos;

    this->mLinkTextButton = nullptr;
}

TextButton::TextButton(class UIScreen* ui, 
        const std::string& name, 
        const std::string& type, 
        const std::map<std::string, std::string>& bindTexName, 
        const std::wstring& text, const Vector3& color, const int& fontSize,
        const int& bindEvent,
        std::function<void()> onClick, 
        const Vector2& pos, 
        const Vector2& dims, 
        const Vector2& scale,
        const bool& addIn
)
: Button(ui, name, type, bindTexName, text, color, fontSize, bindEvent, onClick, pos, dims, scale, addIn)
{
    this->mModText = text;
    this->mMaxTextLength = 50;
    this->mTextBoxColor = Vector3{0.85f, 0.85f, 0.85f};
    this->mTextBGColor = Vector3{0.25f, 0.25f, 0.25f};

    for (auto tex : this->getNameTextures())
    {
        this->mModTextTextures.emplace_back(tex);
    }
    this->mModTextSize = this->getTextSize();

    this->mCurTimestamp = SDL_GetTicks();
    this->mLinkButtonPos = pos;

    this->mLinkTextButton = nullptr;
}

TextButton::~TextButton()
{

}

void TextButton::Rename(class SDL_Window* window, class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader)
{
    SDL_Log("[TextButton] Rename...");
    bool inputting = true;

    SDL_Event event;
    Vector2 mousePos = Vector2{0.0f, 0.0f};
    
    // 设置帧缓存区索引为0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto renderer = this->getUIScreen()->getGame()->getRenderer();
    GLint w = static_cast<int>(renderer->getScreenWidth());
    GLint h = static_cast<int>(renderer->getScreenHeight());
    glViewport(0, 0, w, h);

    // 禁用深度缓存区
    glDisable(GL_DEPTH_TEST);

    // 开启Alpha混合渲染
    // glEnable(GL_BLEND);
    // glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    SDL_Log("[TextButton] Text button pos: (%.1f, %.1f)", this->getPosition().x, this->getPosition().y);
    SDL_Log("[TextButton] Text button box color: (%.2f, %.2f, %.2f)", this->mTextBoxColor.x, this->mTextBoxColor.y, this->mTextBoxColor.z);

    while (inputting)
    {
        
        while (SDL_PollEvent(&event))
        {
            // SDL_Log("[TextButton] key type: %d", event.type);
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                mousePos = Vector2{(float)event.button.x, (float)event.button.y};
                mousePos.x -= this->getUIScreen()->getGame()->getRenderer()->getScreenWidth() * 0.5f;
                mousePos.y = this->getUIScreen()->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

                if (!this->containsPoint(mousePos))
                {   // 鼠标点击文本框外，结束文本输入
                    inputting = false;
                    break;
                }
            }
            else if (event.type == SDL_SCANCODE_DOWN)
            {
                wchar_t wch = event.key.keysym.scancode;
                std::cout << "KEY:" << (char)wch << "\n";
            }
            else if (event.type == SDL_KEYDOWN)
            {
                std::wstring text = this->mModText;
                int len = text.size();
                SDL_Log("[TextButton] text length: %d text sizeX: %.1f", len, this->mModTextSize.x);
                
                switch (event.key.keysym.sym)
                {
                    case SDLK_BACKSPACE:
                    {   // 输入退格键，删掉一个字符
                        if (len > 0)
                        {
                            wchar_t wch = text[len - 1];
                            text[len - 1] = 0;
                            text.resize(len - 1);
                            this->mModText = text;
                            auto iter = this->mModTextTextures.erase(this->mModTextTextures.end() - 1); 
                            Character* ct = this->getUIScreen()->getGame()->getFreeTypeFont()->renderFont(wch, Color::White, 18);

                            // SDL_Log("[TextButton] text size: %.1f", this->mModTextSize.x);
                            float offset = (ct->Advance >> 6);
                            this->mModTextSize.x -= offset;

                            // std::cout << "dele_font: " << (char)(wch) << "\n";
                            SDL_Log("[TextButton] dele font: %c", (char)(wch));
                            SDL_Log("[TextButton] modText: %s text size: %.1f character offset: %.1f", 
                                std::string(this->mModText.begin(), this->mModText.end()).c_str(), this->mModTextSize.x, offset
                            );
                        }
                        break;
                    }
                    case SDLK_RETURN:
                    {
                        inputting = false;
                        break;
                    }
                    default:
                    {
                        if (len < this->mMaxTextLength)
                        {
                            wchar_t wch = event.key.keysym.sym;
                            wchar_t Wch = SDL_GetKeyName(event.key.keysym.sym)[0];

                            SDL_Log("[TextButton] Key %c %c down!", (char)Wch, (char)wch);

                            ++len;
                            text.resize(len);
                            text[len - 1] = wch;
                            // text[len] = 0;
                            this->mModText = text;
                            
                            Character* ch = this->getUIScreen()->getGame()->getFreeTypeFont()->renderFont(wch, Color::White, 18);
                            class Texture* tex = this->getUIScreen()->getGame()->getFreeTypeFont()->getFontTexture(ch, Vector2{this->mModTextSize.x, 0.0f});
                            this->mModTextTextures.emplace_back(tex);
                            float offset = (ch->Advance >> 6);
                            this->mModTextSize.x += offset;

                            SDL_Log("[TextButton] add font: %c", (char)(wch));
                            SDL_Log("[TextButton] modText: %s text size: %.1f character offset: %.1f", 
                                std::string(this->mModText.begin(), this->mModText.end()).c_str(), this->mModTextSize.x, offset
                            );
                        }
                        break;
                    }
                }
            }
        }

        auto pos = this->getPosition() + this->getUIScreen()->getUITranslation();
        auto size = this->getDimension();
        auto tex_offset = this->getBindTexOffset();

        // 修改背景色
        auto bg_centerX = pos.x + (tex_offset.x + 30.0f + size.x * 0.5) * 0.5f;
        auto bg_sizeX = size.x * 0.5 - tex_offset.x - 30.0f;
        Renderers::Graphics2d::drawRectangle(
            basicShader,
            Vector2{bg_centerX, pos.y}, bg_sizeX, size.y, 
            this->mTextBGColor
        );

        // 添加矩形边框
        std::vector<Vector2> points;
        points.emplace_back(Vector2{pos.x + tex_offset.x + 30.0f, pos.y + size.y * 0.5});
        points.emplace_back(Vector2{pos.x + size.x * 0.5, pos.y + size.y * 0.5});
        points.emplace_back(Vector2{pos.x + size.x * 0.5, pos.y - size.y * 0.5});
        points.emplace_back(Vector2{pos.x + tex_offset.x + 30.0f, pos.y - size.y * 0.5});
        Renderers::Graphics2d::drawPolygon(
            basicShader,
            points,
            this->mTextBoxColor,
            2
        );

        // 修改文本
        float dx = -0.5 * this->getTextSize().x;
        float dy = 0.5 * this->getTextSize().y;
        auto font_stX = pos.x + tex_offset.x + 32.0f;
        if (this->mModTextTextures.size() > 0)
        {
            // SDL_Log("[TextButton] text texture size: %d", this->mModTextTextures.size());

            this->getUIScreen()->drawTextures(fontShader, this->mModTextTextures, 
                Vector2{font_stX, pos.y - dy}, 
                1.0f, 1.0f, false, true
            );
        }

        this->mCurTimestamp = SDL_GetTicks();
        if (this->mCurTimestamp % 1000 <= 500)
        {
            // 绘制光标
            float cursorX = 0.0f;
            if (this->mModTextTextures.size() > 0)
            {
                auto tail_font = this->mModTextTextures.back();
                cursorX = font_stX + tail_font->getPosOffset().x + tail_font->getWidth() + 3.0f;
            }
            else
            {
                cursorX = font_stX + 3.0f;
            }
            Renderers::Graphics2d::drawLine(
                basicShader,
                Vector2{cursorX, pos.y - size.y * 0.40f},
                Vector2{cursorX, pos.y + size.y * 0.40f},
                Color::White,
                1
            );
        }
        SDL_GL_SwapWindow(window);

        // SDL_Log("[TextButton] renaming...");
    }

    if (this->checkModTextValid())
    {
        // 替换原来的文本
        this->setText(this->mModText);
        
        // 替换原来的字体纹理
        this->updateTextTexture(this->mModTextTextures);
    }
}

void TextButton::setTextBgColor(const Vector3& color)
{
    this->mTextBGColor = color;
}

void TextButton::setTextBoxColor(const Vector3& color)
{
    this->mTextBoxColor = color;
}

void TextButton::setTextLength(const int& len)
{
    this->mMaxTextLength = len;
}

bool TextButton::checkModTextValid()
{
    bool flag = true;
    for (auto ch : this->mModText)
    {
        if ((int)(ch) > 126 || (int)(ch) < 32)
        {
            std::cout << (char)ch << " " << (int)ch << "\n";
            flag = false;
            break;
        }
    }

    return flag;
}

void TextButton::setLinkButtonPos(const Vector2& pos)
{
    this->mLinkButtonPos = pos;
}

const Vector2& TextButton::getLinkButtonPos() const
{
    return this->mLinkButtonPos;
}

void TextButton::setLinkTextButton(TextButton* tb)
{
    this->mLinkTextButton = tb;
}

TextButton* TextButton::getLinkTextButton()
{
    return this->mLinkTextButton;
}
