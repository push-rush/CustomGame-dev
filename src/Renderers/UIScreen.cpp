#include "../../include/Renderers/UIScreen.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Font.h"
#include "../../include/Renderers/FreeTypeFont.h"
#include "../../include/Renderers/VertexArray.h"
#include "../../include/Renderers/Shader.h"

UIScreen::UIScreen(class Game* game)
{
    this->mGame = game;
    this->mTitlePos = Vector2(0.0f, 250.0f);    
    this->mNextButtonPos = Vector2(0.0f, 150.0f);
    this->mBGPos = Vector2(0.0f, 200.0f);
    this->setUIState(EActive);
    this->mTextColor = Vector3{0.8f, 0.35f, 0.35f};

    // this->mFont = game->getFont("../Assets/Texts/comic.ttf");
    this->mSpriteVerts = createFontVerts(false, Vector2{-0.5f, -0.5f}, 1.0f, 1.0f);

    this->mFreeTypeFont = new FreeTypeFont(game);
    this->mFreeTypeFont->load("../Assets/Texts/Deng.ttf");

    this->mButtonOn = game->getRenderer()->getTexture("../Assets/Images/ButtonYellow.png");
    this->mButtonOff = game->getRenderer()->getTexture("../Assets/Images/ButtonBlue.png");
    this->mSoundSliderOff = game->getRenderer()->getTexture("../Assets/Images/Slider3.png");
    this->mSoundSliderOn = game->getRenderer()->getTexture("../Assets/Images/Slider4.png");
    this->mHookButtonOff = game->getRenderer()->getTexture("../Assets/Images/Hook0.png");
    this->mHookButtonOn = game->getRenderer()->getTexture("../Assets/Images/Hook1.png");
    this->mCrossOff = game->getRenderer()->getTexture("../Assets/Images/Cross2.png"); 
    this->mCrossOn = game->getRenderer()->getTexture("../Assets/Images/Cross3.png"); 
    this->mSoundSlider = game->getRenderer()->getTexture("../Assets/Images/Slider2.png");

    this->mButtonTexs.emplace(RectButtonON, this->mButtonOn);
    this->mButtonTexs.emplace(RectButtonOFF, this->mButtonOff);
    this->mButtonTexs.emplace(SoundSliderOFF, this->mSoundSliderOff);
    this->mButtonTexs.emplace(SoundSliderOn, this->mSoundSliderOn);
    this->mButtonTexs.emplace(HookButtonOFF, this->mHookButtonOff);
    this->mButtonTexs.emplace(HookButtonON, this->mHookButtonOn);
    this->mButtonTexs.emplace(CrossOFF, this->mCrossOff);
    this->mButtonTexs.emplace(CrossOn, this->mCrossOn);
    this->mButtonTexs.emplace(SoundSlider, this->mSoundSlider);

    game->pushUI(this);
}

UIScreen::~UIScreen()
{
    for (auto tex : this->mTitleTextures)
    {
        if (tex)
        {
            tex->unLoad();
            delete tex;
            tex = nullptr;
        }
    }
    this->mTitleTextures.clear();

    for (auto b : mButtons)
    {
        if (b)
        {
            delete b;
            b = nullptr;
        }
    }
    this->mButtons.clear();

    // for (auto bg : this->mBackgrounds)
    // {
    //     if (bg)
    //     {
    //         delete bg;
    //         bg = nullptr;
    //     }
    // }
    // this->mBackgrounds.clear();

    std::cout << "[UIScreen] Releasing..." << std::endl;
}

void UIScreen::update(float dt)
{

}

void UIScreen::draw(class Shader* spriteShader, class Shader* fontShader)
{
    // 禁用深度缓存区
    glDisable(GL_DEPTH_TEST);
    // 开启Alpha混合渲染
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    
    this->mSpriteVerts->setActive();
    
    // 绘制背景
    if (this->mBackgrounds.size() > 0)
    {
        for (auto bg : this->mBackgrounds)
        {
            spriteShader->setActive();
            this->drawTexture(spriteShader, bg, this->mBGPos, bg->getScale().x, bg->getScale().y);
            // SDL_Log("[UIScreen] BG texture path: %s", this->mBackground->getFileName().c_str());
        }
    }
    else
    {
        // SDL_Log("[BG] Texture is null...");
    }

    // 绘制标题
    if (this->mTitleTextures.size() > 0)
    {
        fontShader->setActive();
        fontShader->setVectorUniform("uColor", this->mTextColor);
        // std::cout << "color:" << this->mTextColor.x << " " << this->mTextColor.y << " " << this->mTextColor.z << std::endl;
        
        this->drawTextures(fontShader, this->mTitleTextures, Vector2{this->mTitlePos.x - this->mTextSize.x * 0.5, this->mTitlePos.y - this->mTextSize.y * 0.5}, 1.0f, 1.0f, false, true);
        // std::cout << "Text_Size:(" << this->mTextSize.x << " " << this->mTextSize.y << ")" << std::endl;
        // SDL_Log("[Title] Draw title success...");
    }

    // 绘制按钮
    for (auto b : this->mButtons)
    {
        this->mSpriteVerts->setActive();

        Texture* tex = nullptr;
        auto iter = this->mButtonTexs.find(UIButtonType(b->getButtonType()));

        // std::cout << "type1: " << b->getButtonType() << std::endl;
        if (iter != this->mButtonTexs.end())
        {
            if (!b->getHighlighted()) 
            {
                if (b->getButtonType() + 1 < int(NUM_Buttons))
                {
                    iter = this->mButtonTexs.find(UIButtonType(b->getButtonType() + 1));
                }
            }
            tex = (*iter).second;

            if (tex)
            {
                spriteShader->setActive();
                this->drawTexture(spriteShader, tex, b->getPosition(), b->getButtonScale().x, b->getButtonScale().y);
            }
            else
            {
                SDL_Log("[BG Button] Texture is null");
            }

            if ((b->getNameTextures()).size() > 0)
            {
                // std::cout << "[UIScreen] Contain text " << b->getButtonType() << std::endl;
                
                std::wstring w_text = b->getText();
                std::string str(w_text.begin(), w_text.end());

                // std::cout << "button text:" << str << std::endl;
                // std::cout << "button text:" << std::endl;

                float dx = -0.5 * b->getTextSize().x;
                float dy = 0.5 * b->getTextSize().y;
                
                // 设置文本颜色
                fontShader->setActive();
                fontShader->setVectorUniform("uColor", b->getTextColor());

                // 绘制按钮文本
                this->drawTextures(fontShader, b->getNameTextures(), Vector2{b->getPosition().x + dx, b->getPosition().y - dy}, 1.0f, 1.0f, false, true);
                
                // std::cout << "button_pos:" << b->getPosition().x << " " << b->getPosition().y << std::endl;
                // std::cout << "button_width:" << b->getDimensions().x << " text_length:" << b->getTextLength() << "dx:" << dx << std::endl;
            }
        }
    }
}

void UIScreen::handleKeyPress(int key)
{
    switch (key)
    {
    case (SDL_BUTTON_LEFT):
        if (!this->mButtons.empty())
        {
            for (auto b : this->mButtons)
            {
                if (b->getHighlighted())
                {
                    b->onClick();
                    // break;
                }
            }
        }
        break;
    default:
        break;
    }
}

void UIScreen::close()
{
    this->mState = EClosing;
    // this->getGame()->setGameState(Game::EGamePlay);
    // this->setRelativeMouseMode(true);
}

void UIScreen::setTitleTexture(const std::wstring& text, 
                        const Vector3& color, 
                        int pointSize)
{
    // 清除之前的标题纹理
    for (auto tex : this->mTitleTextures)
    {
        if (tex)
        {   
            tex->unLoad();
            delete tex;
            tex = nullptr;
        }
    }
    this->mTitleTextures.clear();

    // this->mTitleTexture = this->mFont->renderText(text, color, pointSize);
    // wchar_t wtext[text.length()];
    // memcpy(wtext, text.c_str(), sizeof(char) * text.length());

    this->mTextColor = color;
    this->mTextSize = this->mFreeTypeFont->renderText(this->mTitleTextures, text.c_str(), color, pointSize);
    // std::cout << "Size:(" << this->mTextSize.x << " " << this->mTextSize.y << ")" << std::endl;

    if (this->mTitleTextures.size() > 0)
    {
        SDL_Log("[UIScreen] Set title texture success!!!");
    }
    else
    {
        SDL_Log("[UIScreen] Set title texture failed!!!");
    }
}

UIScreen::UIState UIScreen::getState() const
{
    return this->mState;
}

void UIScreen::drawTexture(class Shader* shader, class Texture* texture, const Vector2& offset, 
    float scaleX, float scaleY, bool flipY, bool drawFont)
{
    shader->setActive();
    if (texture)
    {
        // 判断是否需要翻转y坐标
        float yScale = static_cast<float>(texture->getHeight() * scaleY);
        if (flipY)
        {
            yScale *= -1.0f;
        }

        Matrix4 scaleMat = Matrix4::CreateScale(((float)texture->getWidth()) * scaleX, yScale, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
        if (drawFont)
        {
            scaleMat = Matrix4::CreateScale(scaleX, scaleY, 1.0f);
            transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        }

        Matrix4 world = scaleMat * transMat;
        texture->setActive();
        shader->setMatrixUniform("uWorldTransform", world);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        // SDL_Log("[UIScreen] Texture is null...");
    }
}

void UIScreen::drawTextures(class Shader* shader, std::vector<class Texture*>& textures,
    const Vector2& offset, float scaleX, float scaleY, bool flipY, bool drawFont)
{
    // // 设置字体颜色
    shader->setActive();
    // shader->setVectorUniform("uColor", this->mTextColor);
    
    for (auto tex : textures)
    {
        this->mFontVerts = createFontVerts(true, offset + tex->getPosOffset(), tex->getWidth(), tex->getHeight());
        this->mFontVerts->setActive();
        drawTexture(shader, tex, offset, scaleX, scaleY, flipY, drawFont);
    }
}

Game* UIScreen::getGame() const
{
    return this->mGame;    
}

void UIScreen::addButton(const UIButtonType& type, const std::wstring& name, const Vector3& color, std::function<void()> onClick, const Vector2& scale)
{
    Vector2 dims(static_cast<float>(this->mButtonOn->getWidth()), static_cast<float>(this->mButtonOn->getHeight()));
    auto iter = this->mButtonTexs.find(type);
    if (iter != this->mButtonTexs.end())
    {
        dims = Vector2{(float)((*iter).second->getWidth()), (float)((*iter).second->getHeight())};
    }

    // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
    Button* b = new Button(int(type), name, this->mFreeTypeFont, color, onClick, this->mNextButtonPos, dims, scale);
    this->mButtons.emplace_back(b);

    this->mNextButtonPos.y -= (static_cast<float>(this->mButtonOff->getHeight()) + 20.0f);
}

void UIScreen::addButton(const UIButtonType& type, const std::wstring& name, 
    const Vector3& color, const Vector2& pos, std::function<void()> onClick, const Vector2& scale)
{
    this->setButtonPos(pos);

    Vector2 dims(static_cast<float>(this->mButtonOn->getWidth()), static_cast<float>(this->mButtonOn->getHeight()));
    auto iter = this->mButtonTexs.find(type);
    if (iter != this->mButtonTexs.end())
    {
        dims = Vector2{(float)((*iter).second->getWidth()), (float)((*iter).second->getHeight())};
    }

    // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
    Button* b = new Button(int(type), name, this->mFreeTypeFont, color, onClick, pos, dims, scale);
    this->mButtons.emplace_back(b);
}

void UIScreen::drawScreen()
{
    Shader* spriteShader = this->getGame()->getRenderer()->getSpriteShader();
    Shader* fontShader = this->getGame()->getRenderer()->getFontShader();

    for (auto b : this->mButtons)
    {
        Texture* tex = nullptr;
        auto iter = this->mButtonTexs.find(UIButtonType(b->getButtonType()));

        // std::cout << "type2: " << b->getButtonType() << std::endl;
        if (iter != this->mButtonTexs.end())
        {
            if (!b->getHighlighted())
            {
                iter = this->mButtonTexs.find(UIButtonType(b->getButtonType() + 1));
            }
            tex = (*iter).second;

            if (tex)
            {
                // 绘制按钮背景纹理
                this->drawTexture(spriteShader, tex, b->getPosition());
            }
            else
            {
                SDL_Log("[BG button] Texture is null...");
            }
            
            if ((b->getNameTextures()).size() > 0)
            {
                // std::cout << "Contain text..." << std::endl;

                // 绘制按钮纹理
                float dx = -0.5 * b->getTextSize().x;
                float dy = 0.5 * b->getTextSize().y;
                this->drawTextures(fontShader, b->getNameTextures(), Vector2{b->getPosition().x + dx, b->getPosition().y - dy}, 1.0f, 1.0f, false, true);
            }
            else
            {
                SDL_Log("[button] Texture is null...");
            }
        }
    }
}

void UIScreen::processInput(const uint8_t* keys)
{
    if (!this->mButtons.empty())
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
        mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
        mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

        for (auto b : this->mButtons)
        {
            if (b->containsPoint(mousePos))
            {
                b->setHighlighted(true);
            }
            else
            {
                b->setHighlighted(false);
            }
        }
    }
}

void UIScreen::setFont(class Font* font)
{
    this->mFont = font;
}

void UIScreen::setTitlePos(Vector2 titlePos)
{   
    this->mTitlePos = titlePos;
}   

void UIScreen::setUIState(UIState state)
{
    this->mState = state;
}

void UIScreen::setBGPos(Vector2 bgPos)
{
    this->mBGPos = bgPos;
}

void UIScreen::setNextButtonPos(Vector2 nextButtonPos)
{
    this->mNextButtonPos = nextButtonPos;
}

void UIScreen::addBackground(Texture* tex)
{   
    this->mBackgrounds.emplace_back(tex);
}

class Font* UIScreen::getFont() const
{
    return this->mFont;
}

std::vector<class Texture*>& UIScreen::getTexture()
{
    return this->mTitleTextures;
}

Vector2 UIScreen::getTitlePos() const
{
    return this->mTitlePos;
}

UIScreen::UIState UIScreen::getUIState() const
{
    return this->mState;
}

Vector2 UIScreen::getBGPos() const
{
    return this->mBGPos;
}

Vector2 UIScreen::getNextButtonPos() const
{
    return this->mNextButtonPos;
}

std::vector<Texture*>& UIScreen::getBackgrounds()
{
    return this->mBackgrounds;
}

void UIScreen::setRelativeMouseMode(bool relative)
{
    if (relative)
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_GetRelativeMouseState(nullptr, nullptr);
    }
    else
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

VertexArray* UIScreen::createFontVerts(bool draw_font, const Vector2& pos, const float& w, const float& h)
{
    float* vertexBuffer = new float[32];
    if (draw_font)
    {
        float x1 = pos.x;
        float x2 = pos.x + w;

        float y1 = pos.y;
        float y2 = pos.y + h;

        float loc[] = {
            x1, y2, 0.0f, 0.f, 0.f, 0.f,  0.f,  0.f,
            x2, y2, 0.0f, 0.f, 0.f, 0.f, 1.0f, 0.0f,
            x2, y1, 0.0f, 0.f, 0.f, 0.f, 1.0f, 1.0f,
            x1, y1, 0.0f, 0.f, 0.f, 0.f, 0.0f, 1.0f
        };

        memcpy(vertexBuffer, loc, sizeof(float) * 32);
        // std::cout << "pos: x(" << x1 << ", " << x2 << ") y(" << y1 << ", " << y2 << ")" << std::endl;
    }
    else
    {
        float loc[] = {
            -0.5f,  0.5f, 0.0f, 0.f, 0.f, 0.f,  0.f,  0.f,
             0.5f,  0.5f, 0.0f, 0.f, 0.f, 0.f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.f, 0.f, 0.f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.f, 0.f, 0.f, 0.0f, 1.0f
        };
        memcpy(vertexBuffer, loc, sizeof(float) * 32);
    }

    uint32_t indexBuffer[] = {
        0, 1, 2,
        2, 3, 0
    };

    return (new VertexArray(VertexArray::PosNormTex, vertexBuffer, 4, indexBuffer, 6));
}

void UIScreen::setTextColor(const Vector3& color)
{
    this->mTextColor = color;
}

Vector3 UIScreen::getTextColor() const
{
    return this->mTextColor;
}

void UIScreen::setButtonPos(const Vector2& pos)
{
    this->mNextButtonPos = pos;
}

Vector2 UIScreen::getButtonPos() const
{
    return this->mNextButtonPos;
}

void UIScreen::addButton(const UIButtonType& type, const Vector2& pos, const Vector2& scale)
{
    this->setButtonPos(pos);

    Vector2 dims(static_cast<float>(this->mButtonOn->getWidth()), static_cast<float>(this->mButtonOn->getHeight()));
    auto iter = this->mButtonTexs.find(type);
    if (iter != this->mButtonTexs.end())
    {
        dims = Vector2{(float)((*iter).second->getWidth()), (float)((*iter).second->getHeight())};
    }
    
    // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
    Button* b = new Button(int(type), pos, dims, scale);
    this->mButtons.emplace_back(b);
}

void UIScreen::addButton(const UIButtonType& type, const Vector2& pos, std::function<void()> onClick, const Vector2& scale)
{
    this->setButtonPos(pos);

    Vector2 dims(static_cast<float>(this->mButtonOn->getWidth()), static_cast<float>(this->mButtonOn->getHeight()));
    auto iter = this->mButtonTexs.find(type);
    if (iter != this->mButtonTexs.end())
    {
        dims = Vector2{(float)((*iter).second->getWidth()), (float)((*iter).second->getHeight())};
    }

    // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
    Button* b = new Button(int(type), onClick, pos, dims, scale);
    this->mButtons.emplace_back(b);
}

std::unordered_map<UIScreen::UIButtonType, class Texture*>& UIScreen::getButtonTexs()
{   
    return this->mButtonTexs;
}

std::vector<class Button*>& UIScreen::getButtons()
{
    return this->mButtons;
}

UIScreen::UIType UIScreen::getUIType() const
{
    return this->mUIType;
}

void UIScreen::setUIType(UIScreen::UIType type)
{
    this->mUIType = type;
}