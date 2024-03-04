#include "../../include/Renderers/UIScreen.h"

#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Font.h"
#include "../../include/Renderers/FreeTypeFont.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/TextButton.h"

#include "../../include/Game.h"

#include "../../include/Renderers/Graphics2d.h"

UIScreen::UIScreen(class Game* game)
{
    this->mGame = game;
    this->mTitlePos = Vector2(0.0f, 250.0f);    
    this->mNextButtonPos = Vector2(0.0f, 150.0f);
    this->mBGPos = Vector2(0.0f, 200.0f);
    this->setUIState(EActive);
    this->mTextColor = Vector3{0.8f, 0.35f, 0.35f};
    
    this->mUIPosOffset = Vector2{0.0f, 0.0f};
    this->mUIViewScale = Vector2{1.0f, 1.0f};
    this->mUIBGColor = Vector3{0.5f, 0.5f, 0.5f};

    // this->mFont = game->getFont("../Assets/Texts/comic.ttf");
    this->mSpriteVerts = createFontVerts(false, Vector2{-0.5f, -0.5f}, 1.0f, 1.0f);

    this->mUITranslation = Vector2{0.0f, 0.0f};
    this->mPosOffsetCoeff = 0.0f;

    // this->mFreeTypeFont = new FreeTypeFont(game);
    // this->mFreeTypeFont->load("../Assets/Texts/Deng.ttf");

    // game->pushUI(this);
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

    Texture* temp = nullptr;
    for (auto tex : this->mUITextures)
    {
        temp = tex.second;
        if (temp)
        {
            delete temp;
            temp = nullptr;
        }
    }
    this->mUITextures.clear();

    std::cout << "[UIScreen] Releasing..." << std::endl;
}

void UIScreen::update(float dt)
{
    int x = 0, y = 0;
    Uint32 mouse = SDL_GetMouseState(&x, &y);
    if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        // SDL_Log("[UIScreen] Button left is pressed...");
    }
}

void UIScreen::draw(
    class Shader* basicShader, class Shader* spriteShader, 
    class Shader* fontShader, 
    class EmptySprite* elem
)
{
    auto pos_offset = this->getUITranslation();

    if (basicShader) 
    {
        if (!strcmp(elem->getType().c_str(), "button"))
        {
            Button* b = (Button*)(elem);
            if (b->getHighlighted() || b->getSpriteState() == EmptySprite::ESelected)
            {
                Renderers::Graphics2d::drawRectangle(
                    basicShader, 
                    b->getPosition() + pos_offset, 
                    b->getDimension().x, b->getDimension().y, 
                    b->getBindBgColor()
                );
                Renderers::Graphics2d::drawPolygon(
                    basicShader, 
                    { 
                        Vector2{b->getPosition().x - b->getDimension().x * 0.5f + pos_offset.x, b->getPosition().y + b->getDimension().y * 0.5f + pos_offset.y}, 
                        Vector2{b->getPosition().x + b->getDimension().x * 0.5f + pos_offset.x, b->getPosition().y + b->getDimension().y * 0.5f + pos_offset.y},
                        Vector2{b->getPosition().x + b->getDimension().x * 0.5f + pos_offset.x, b->getPosition().y - b->getDimension().y * 0.5f + pos_offset.y},
                        Vector2{b->getPosition().x - b->getDimension().x * 0.5f + pos_offset.x, b->getPosition().y - b->getDimension().y * 0.5f + pos_offset.y}
                    },
                    b->getBindBoxColor(),
                    2
                );
            }
        }
        else if (elem->getBindTexName().size() < 1)
        {
            Renderers::Graphics2d::drawRectangle(
                basicShader, 
                elem->getPosition() + pos_offset, 
                elem->getDimension().x, elem->getDimension().y, 
                elem->getBindBgColor()
            );
        }
    }

    if (spriteShader && elem->getBindTexName().size() > 0)
    {
        std::string bind_tex = "default";
        if (elem->getBindTexName().find("on") != elem->getBindTexName().end() &&
            elem->getBindTexName().find("off") != elem->getBindTexName().end())
        {
            Button* b = (Button*)(elem);
            bind_tex = b->getHighlighted() ? 
                (*b->getBindTexName().find("on")).second : (*b->getBindTexName().find("off")).second;
        }
        else
        {
            bind_tex = (*elem->getBindTexName().begin()).second;
        }

        auto iter = this->mUITextures.find(bind_tex);
        if (iter != this->mUITextures.end())
        {
            this->mSpriteVerts->setActive();
            this->drawTexture(spriteShader, (*iter).second, elem->getPosition() + elem->getBindTexOffset() + pos_offset);
        }
        else
        {
            SDL_Log("[UIScreen] tex is not find: %s %s", 
                elem->getSpriteName().c_str(),
                bind_tex.c_str()
            );
        }
    }
    
    if (fontShader && elem->getNameTextures().size() > 0)
    {
        std::wstring w_text = elem->getText();
        std::string str(w_text.begin(), w_text.end());

        float dx = -0.5 * elem->getTextSize().x;
        float dy = 0.5 * elem->getTextSize().y;
        
        // 激活文本着色器
        fontShader->setActive();
        // 设置文本颜色
        fontShader->setVectorUniform("uColor", elem->getTextColor());

        // 绘制按钮文本
        this->drawTextures(fontShader, elem->getNameTextures(), 
            Vector2{elem->getPosition().x + dx + elem->getBindTextOffset().x + pos_offset.x, 
                elem->getPosition().y - dy + elem->getBindTextOffset().y + pos_offset.y
            }, 
            1.0f, 1.0f, false, true
        );
    }
}

// void UIScreen::draw(class Shader* spriteShader, class Shader* fontShader, EmptySprite* elem, const Vector2& offset)
// {
//     // SDL_Log("[UIScreen] drawing...");

//     // 禁用深度缓存区
//     // glDisable(GL_DEPTH_TEST);
//     // // 开启Alpha混合渲染
//     // glEnable(GL_BLEND);
//     // glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//     // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

//     // if (sorted)
//     // {
//     //     std::sort(elems.begin(), elems.end(), [](EmptySprite* e1, EmptySprite* e2)
//     //     {
//     //         return e1->getUpdateOrder() < e2->getUpdateOrder();
//     //     });
//     // }
    
//     // 绘制ui元素
//     // for (auto elem : elems)
//     // {
//         // // 激活精灵着色器

//     if (elem)
//     {
//         spriteShader->setActive();
//         this->mSpriteVerts->setActive();

//         std::string bind_tex = (*elem->getBindTexName().begin()).second;
//         if (!strcmp(elem->getType().c_str(), "button"))
//         {
//             Button* b = reinterpret_cast<Button*>(elem);
//             // SDL_Log("[UIScreen] SPRITE NAME: %s", b->getSpriteName().c_str());

//             if (b->getBindTexName().find("on") != b->getBindTexName().end() &&
//                 b->getBindTexName().find("off") != b->getBindTexName().end()
//             )
//             {
//                 bind_tex = b->getHighlighted() ? 
//                     (*b->getBindTexName().find("on")).second : (*b->getBindTexName().find("off")).second;
//             }
//             else
//             {
//                 SDL_Log("[UIScreen] bind tex is null: %s", b->getSpriteName().c_str());
//             }
//         }

//         auto iter = this->mUITextures.find(bind_tex);
//         if (iter != this->mUITextures.end())
//         {
//             auto tex = (*iter).second;
//             if (tex)
//             {
//                 // SDL_Log("[UIScreen] draw texture name: %s", tex->getFileName().c_str());
//                 this->drawTexture(spriteShader, tex, elem->getPosition());
//             }
//             else
//             {
//                 SDL_Log("[UIScreen] tex is null: %s", elem->getSpriteName().c_str());
//             }
//         }
//         else
//         {
//             SDL_Log("[UIScreen] tex is not find: %s", elem->getSpriteName().c_str());
//         }

//         if (elem->getNameTextures().size() > 0)
//         {
//             std::wstring w_text = elem->getText();
//             std::string str(w_text.begin(), w_text.end());

//             float dx = -0.5 * elem->getTextSize().x;
//             float dy = 0.5 * elem->getTextSize().y;
            
//             // 激活文本着色器
//             fontShader->setActive();
//             // 设置文本颜色
//             fontShader->setVectorUniform("uColor", elem->getTextColor());

//             // 绘制按钮文本
//             this->drawTextures(fontShader, elem->getNameTextures(), 
//                 Vector2{elem->getPosition().x + dx + offset.x, elem->getPosition().y - dy + offset.y}, 
//                 1.0f, 1.0f, false, true
//             );
//         }
//     }
//     // else
//     // {
//     //     SDL_Log("[UIScreen] elem is null...");
//     // }

//     // }

//     // std::vector<Texture*> fonts;
//     // for (auto elem : this->mUIElements)
//     // {
//     //     auto b = elem.second->mBindButton;
//     //     if (b)
//     //     {
//     //         spriteShader->setActive();
//     //         this->mSpriteVerts->setActive();
            
//     //         auto texs = elem.second->mTextures;
//     //         std::string type = b->getHighlighted() ? "on" : "off";
//     //         Texture* selected = nullptr;
//     //         for (auto tex : texs)
//     //         {
//     //             if (tex->getTexType() == type)
//     //             {
//     //                 selected = tex;
//     //                 break;
//     //             }
//     //         }

//     //         if (selected)
//     //         {
//     //             this->drawTexture(spriteShader, selected, 
//     //                 b->getPosition() + selected->getPosOffset(), 
//     //                 selected->getScale().x, selected->getScale().y
//     //             );
//     //         }

//     //         auto text = elem.second->mBindText;
//     //         if (text)
//     //         {
//     //             Vector2 size = this->mFreeTypeFont->renderText(fonts, (*text).c_str());
//     //             float dx = -0.5 * size.x;
//     //             float dy = 0.5 * size.y;

//     //             // 设置文本颜色
//     //             fontShader->setActive();
//     //             fontShader->setVectorUniform("uColor", Color::Red);
//     //             this->drawTextures(fontShader, fonts, 
//     //                 Vector2{b->getPosition().x + dx, b->getPosition().y - dy}, 
//     //                 1.0f, 1.0f, false, true
//     //             );
//     //         }
//     //     }
//     //     else
//     //     {
//     //         spriteShader->setActive();
//     //         this->mSpriteVerts->setActive();
            
//     //         auto texs = elem.second->mTextures;
//     //         Texture* selected = nullptr;
//     //         if (texs.size() > 0)
//     //         {
//     //             selected = texs[0];
//     //             this->drawTexture(
//     //                 spriteShader, 
//     //                 selected, 
//     //                 selected->getPosOffset(), 
//     //                 selected->getScale().x, 
//     //                 selected->getScale().y
//     //             );
//     //         }

//     //         if (selected)
//     //         {
//     //             auto text = elem.second->mBindText;
//     //             if (text)
//     //             {
//     //                 Vector2 size = this->mFreeTypeFont->renderText(fonts, (*text).c_str());
//     //                 float dx = -0.5 * size.x;
//     //                 float dy = 0.5 * size.y;

//     //                 // 设置文本颜色
//     //                 // fontShader->setActive();
//     //                 // fontShader->setVectorUniform("uColor", );
                    
//     //                 this->drawTextures(fontShader, fonts, 
//     //                     Vector2{selected->getPosOffset().x + dx, 
//     //                         selected->getPosOffset().y - dy
//     //                     }, 1.0f, 1.0f, false, true
//     //                 );
//     //             }
//     //         }
//     //     }
//     //     fonts.clear();
//     // }

//     // // 绘制背景
//     // auto iter = this->mUIElements.find("Background");
//     // if (iter != this->mUIElements.end())
//     // {
//     //     spriteShader->setActive();
//     //     this->mSpriteVerts->setActive();

//     //     auto bgs = (*iter).second;
//     //     for (auto bg : bgs->mTextures)
//     //     {
//     //         this->drawTexture(spriteShader, bg, this->mBGPos, bg->getScale().x, bg->getScale().y);
//     //     }
//     // }

//     // // 绘制标题
//     // if (this->mTitleTextures.size() > 0)
//     // {
//     //     fontShader->setActive();
//     //     fontShader->setVectorUniform("uColor", this->mTextColor);
        
//     //     this->drawTextures(fontShader, this->mTitleTextures, 
//     //         Vector2{this->mTitlePos.x - this->mTextSize.x * 0.5, 
//     //             this->mTitlePos.y - this->mTextSize.y * 0.5
//     //         }, 1.0f, 1.0f, false, true
//     //     );
//     // }

//     // for (auto b : this->mButtons)
//     // {
//     //     this->mSpriteVerts->setActive();

//     //     Texture* tex = b->getHighlighted() ? b->getButtonOn() : b->getButtonOff();
//     //     // if (!tex)
//     //     // {
//     //     //     auto iter = this->mButtonTexs.find(UIButtonType(b->getButtonType()));
//     //     //     if (iter != this->mButtonTexs.end())
//     //     //     {
//     //     //         auto texOn = (*iter).second;
//     //     //         if (!b->getHighlighted()) 
//     //     //         {
//     //     //             if (b->getButtonType() + 1 < int(NUM_Buttons))
//     //     //             {
//     //     //                 iter = this->mButtonTexs.find(UIButtonType(b->getButtonType() + 1));
//     //     //             }
//     //     //         }
//     //     //         tex = (*iter).second;

//     //     //         b->setButtonTex(texOn, tex);
//     //     //     }
//     //     // }

//     //     // std::cout << "type1: " << b->getButtonType() << std::endl;
//     //     if (tex)
//     //     {
//     //         spriteShader->setActive();
//     //         this->drawTexture(spriteShader, tex, b->getPosition(), b->getButtonScale().x, b->getButtonScale().y);
           
//     //         if ((b->getNameTextures()).size() > 0)
//     //         {
//     //             // std::cout << "[UIScreen] Contain text " << b->getButtonType() << std::endl;
                
//     //             std::wstring w_text = b->getText();
//     //             std::string str(w_text.begin(), w_text.end());

//     //             // std::cout << "button text:" << str << std::endl;
//     //             // std::cout << "button text:" << std::endl;

//     //             float dx = -0.5 * b->getTextSize().x;
//     //             float dy = 0.5 * b->getTextSize().y;
                
//     //             // 设置文本颜色
//     //             fontShader->setActive();
//     //             fontShader->setVectorUniform("uColor", b->getTextColor());

//     //             // 绘制按钮文本
//     //             this->drawTextures(fontShader, b->getNameTextures(), Vector2{b->getPosition().x + dx, b->getPosition().y - dy}, 1.0f, 1.0f, false, true);
                
//     //             // std::cout << "button_pos:" << b->getPosition().x << " " << b->getPosition().y << std::endl;
//     //             // std::cout << "button_width:" << b->getDimensions().x << " text_length:" << b->getTextLength() << "dx:" << dx << std::endl;
//     //         }
//     //     }
//     // }
// }

void UIScreen::handleMouseWheel(const int& mouse_wheel)
{



}

void UIScreen::handleKeyPress(int key)
{
    SDL_Log("[UIScreen] type: %d key: %d", (int)this->getUIType(), key);
    
    switch (key)
    {
    case (SDL_BUTTON_LEFT):
    {
        // if (!this->mButtons.empty())
        // {
        //     for (auto b : this->mButtons)
        //     {
        //         if (b->getHighlighted())
        //         {
        //             b->onClick();
        //             // break;
        //         }
        //     }
        // }

        if (!this->mUIElements.empty())
        {
            for (auto elem : this->mUIElements)
            {
                if (!strcmp(elem->getType().c_str(), "button"))
                {
                    auto b = (Button*)(elem);
                    if (b && b->getHighlighted())
                    {
                        b->onClick();
                        // break;

                        SDL_Log("[UIScreen] Click ui type: %d", this->mUIType);
                    }
                }
            }
        }
        break;
    }
    case (SDLK_ESCAPE):
    {
        // 退出当前ui菜单
        this->close();
    }
    default:
        break;
    }
}

void UIScreen::close()
{
    // this->mState = EClosing;
    this->mState = EInvisible;

    this->getGame()->setGameState(Game::EGamePlay);
    this->setRelativeMouseMode(true);
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
    this->mTextSize = this->getGame()->getFreeTypeFont()->renderText(this->mTitleTextures, text.c_str(), color, pointSize);
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
        // (createFontVerts(true, offset + tex->getPosOffset(), tex->getWidth(), tex->getHeight())).setActive();

        drawTexture(shader, tex, offset, scaleX, scaleY, flipY, drawFont);
       
        if (this->mFontVerts)
        {
            delete this->mFontVerts;
            this->mFontVerts = nullptr;
        }
    }
}

Game* UIScreen::getGame() const
{
    return this->mGame;    
}

template <typename T>
T* UIScreen::addButton(
    const std::string& name, const std::string& type, 
    const std::map<std::string, std::string>& bindTex,
    const std::wstring& text, const Vector3& color, const int& fontSize,
    const Vector2& pos, const UIBindEvent& bind_event, 
    std::function<void()> onClick, const bool& addIn, const Vector2& scale
)
{
    // this->setButtonPos(pos);

    class Texture* bn_tex = nullptr;
    auto iter = this->mUITextures.find((*bindTex.begin()).second);
    if (iter != this->mUITextures.end())
    {
        bn_tex = (*iter).second;
    }

    if (bn_tex)
    {
        Vector2 dims(static_cast<float>(bn_tex->getWidth()), static_cast<float>(bn_tex->getHeight()));

        // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
        // Button* b = new Button(this, int(type), name, this->mFreeTypeFont, color, onClick, this->mNextButtonPos, dims, scale);

        SDL_Log("dim: (%.2f, %.2f)", dims.x, dims.y);

        T* b = new T(
            this, name, type, 
            bindTex,
            text, color, fontSize,
            int(bind_event),
            onClick,
            pos, dims, scale, 
            addIn
        );

        // this->mButtons.emplace_back(b);

        this->mNextButtonPos.y -= (static_cast<float>(bn_tex->getHeight()) + 20.0f);

        return b;
    }

    return nullptr;
}

template <typename T>
T* UIScreen::addButton(
    const std::string& name, const std::string& type, 
    const std::map<std::string, std::string>& bindTex,
    const Vector2& pos, 
    const UIBindEvent& bind_event, std::function<void()> onClick, 
    const bool& addIn, 
    const Vector2& scale)
{
    // this->setButtonPos(pos);

    // Vector2 dims(static_cast<float>(this->mButtonOn->getWidth()), static_cast<float>(this->mButtonOn->getHeight()));
    // auto iter = this->mButtonTexs.find(type);
    // if (iter != this->mButtonTexs.end())
    // {
    //     dims = Vector2{(float)((*iter).second->getWidth()), (float)((*iter).second->getHeight())};
    // }

    // // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
    // Button* b = new Button(this, int(type), name, this->mFreeTypeFont, color, onClick, pos, dims, scale);
    // this->mButtons.emplace_back(b);

    class Texture* bn_tex = nullptr;
    auto iter = this->mUITextures.find((*bindTex.begin()).second);
    if (iter != this->mUITextures.end())
    {
        bn_tex = (*iter).second;
    }

    if (bn_tex)
    {
        Vector2 dims(static_cast<float>(bn_tex->getWidth()), static_cast<float>(bn_tex->getHeight()));

        SDL_Log("dim: (%.2f, %.2f)", dims.x, dims.y);

        // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
        // Button* b = new Button(this, int(type), name, this->mFreeTypeFont, color, onClick, this->mNextButtonPos, dims, scale);

        T* b = new T(
            this, name, type, 
            bindTex,
            int(bind_event),
            onClick, 
            pos, dims, scale,
            addIn
        );

        // this->mButtons.emplace_back(b);

        this->mNextButtonPos.y -= (static_cast<float>(bn_tex->getHeight()) + 20.0f);

        return b;
    }

    return nullptr;
}

// void UIScreen::drawScreen()
// {
//     Shader* spriteShader = this->getGame()->getRenderer()->getSpriteShader();
//     Shader* fontShader = this->getGame()->getRenderer()->getFontShader();

//     for (auto b : this->mButtons)
//     {
//         Texture* tex = nullptr;
//         auto iter = this->mButtonTexs.find(UIButtonType(b->getButtonType()));

//         // std::cout << "type2: " << b->getButtonType() << std::endl;
//         if (iter != this->mButtonTexs.end())
//         {
//             if (!b->getHighlighted())
//             {
//                 iter = this->mButtonTexs.find(UIButtonType(b->getButtonType() + 1));
//             }
//             tex = (*iter).second;

//             if (tex)
//             {
//                 // 绘制按钮背景纹理
//                 this->drawTexture(spriteShader, tex, b->getPosition());
//             }
//             else
//             {
//                 SDL_Log("[BG button] Texture is null...");
//             }
            
//             if ((b->getNameTextures()).size() > 0)
//             {
//                 // std::cout << "Contain text..." << std::endl;

//                 // 绘制按钮纹理
//                 float dx = -0.5 * b->getTextSize().x;
//                 float dy = 0.5 * b->getTextSize().y;
//                 this->drawTextures(fontShader, b->getNameTextures(), Vector2{b->getPosition().x + dx, b->getPosition().y - dy}, 1.0f, 1.0f, false, true);
//             }
//             else
//             {
//                 SDL_Log("[button] Texture is null...");
//             }
//         }
//     }
// }

void UIScreen::processInput(const uint8_t* keys)
{
    // if (!this->mButtons.empty())
    // {
    //     int x, y;
    //     SDL_GetMouseState(&x, &y);
    //     Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    //     mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
    //     mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

    //     for (auto b : this->mButtons)
    //     {
    //         if (b->containsPoint(mousePos))
    //         {
    //             b->setHighlighted(true);
    //         }
    //         else
    //         {
    //             b->setHighlighted(false);
    //         }
    //     }
    // }

    if (!this->mUIElements.empty())
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
        mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
        mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

        for (auto elem : this->mUIElements)
        {
            if (!strcmp(elem->getType().c_str(), "button"))
            {
                Button* b = reinterpret_cast<Button*>(elem);
                if (b->containsPoint(mousePos))
                {
                    b->setHighlighted(true);
                    // SDL_Log("[UIScreen] button: %s is highlighted...", b->getSpriteName().c_str());
                }
                else
                {
                    b->setHighlighted(false);
                }

                // SDL_Log("[UIScreen] button STATE: %d MOUSE pos: (%d, %d) button pos: (%.2f, %.2f)", 
                    // (int)b->getHighlighted(), x, y, b->getPosition().x, b->getPosition().y);
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

void UIScreen::setUIState(const UIState& state)
{
    this->mState = state;
}

void UIScreen::setBGPos(Vector2 bgPos)
{
    this->mBGPos = bgPos;
}

// void UIScreen::setNextButtonPos(Vector2 nextButtonPos)
// {
//     this->mNextButtonPos = nextButtonPos;
// }

// void UIScreen::addBackground(Texture* tex)
// {   
//     this->mBackgrounds.emplace_back(tex);
// }

class Font* UIScreen::getFont() const
{
    return this->mFont;
}

// std::vector<class Texture*>& UIScreen::getTexture()
// {
//     return this->mTitleTextures;
// }

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

// Vector2 UIScreen::getNextButtonPos() const
// {
//     return this->mNextButtonPos;
// }

// std::vector<Texture*>& UIScreen::getBackgrounds()
// {
//     return this->mBackgrounds;
// }

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
    float vertexBuffer[32];
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

    return new VertexArray{VertexArray::PosNormTex, vertexBuffer, 4, indexBuffer, 6};
}

void UIScreen::setTextColor(const Vector3& color)
{
    this->mTextColor = color;
}

Vector3 UIScreen::getTextColor() const
{
    return this->mTextColor;
}

// void UIScreen::setButtonPos(const Vector2& pos)
// {
//     this->mNextButtonPos = pos;
// }

// Vector2 UIScreen::getButtonPos() const
// {
//     return this->mNextButtonPos;
// }

// void UIScreen::addButton(const std::string& type, const Vector2& pos, const Vector2& scale)
// {
//     this->setButtonPos(pos);

//     Vector2 dims(static_cast<float>(this->mButtonOn->getWidth()), static_cast<float>(this->mButtonOn->getHeight()));
//     auto iter = this->mButtonTexs.find(type);
//     if (iter != this->mButtonTexs.end())
//     {
//         dims = Vector2{(float)((*iter).second->getWidth()), (float)((*iter).second->getHeight())};
//     }
    
//     // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
//     Button* b = new Button(this, int(type), pos, dims, scale);
//     this->mButtons.emplace_back(b);
// }

// void UIScreen::addButton(const std::string& type, const Vector2& pos, std::function<void()> onClick, const Vector2& scale)
// {
//     this->setButtonPos(pos);

//     Vector2 dims(static_cast<float>(this->mButtonOn->getWidth()), static_cast<float>(this->mButtonOn->getHeight()));
//     auto iter = this->mButtonTexs.find(type);
//     if (iter != this->mButtonTexs.end())
//     {
//         dims = Vector2{(float)((*iter).second->getWidth()), (float)((*iter).second->getHeight())};
//     }

//     // Button* b = new Button(name, this->mFont, onClick, this->mNextButtonPos, dims);
//     Button* b = new Button(this, int(type), onClick, pos, dims, scale);
//     this->mButtons.emplace_back(b);
// }

// std::unordered_map<UIScreen::UIButtonType, class Texture*>& UIScreen::getButtonTexs()
// {   
//     return this->mButtonTexs;
// }

// std::vector<class Button*>& UIScreen::getButtons()
// {
//     return this->mButtons;
// }

UIScreen::UIType UIScreen::getUIType() const
{
    return this->mUIType;
}

void UIScreen::setUIType(UIScreen::UIType type)
{
    this->mUIType = type;
}

std::vector<EmptySprite*>& UIScreen::getUIElements()
{
    return this->mUIElements;
}   

void UIScreen::addUIElement(class EmptySprite* sprite)
{
    this->mUIElements.emplace_back(sprite);
}

void UIScreen::removeElement(class EmptySprite* sprite)
{
    for (auto elem : this->mUIElements)
    {
        if (elem == sprite)
        {
            delete elem;
            elem = nullptr;
            break;
        }
    }
}

void UIScreen::addUITexture(const std::string& name, class Texture* tex)
{
    this->mUITextures.emplace(name, tex);
}

void UIScreen::removeTexture(const std::string& name)
{
    auto iter = this->mUITextures.find(name);

    if (iter != this->mUITextures.end())
    {
        Texture* temp = nullptr;
        temp = (*iter).second;
        iter = this->mUITextures.erase(iter);
        if (temp)
        {
            delete temp;
            temp = nullptr;
        }
    }
}

std::unordered_map<std::string, class Texture*>& UIScreen::getUITextures()
{
    return this->mUITextures;
}

void UIScreen::bindEvent(const UIBindEvent& event, Button* b)
{
    SDL_Log("[UIScreen] bind event id: %d", (int)event);
    switch (event)
    {
        case UICancel:
        {
            this->close();
            break;
        }
        case UIConfirm:
        {
            this->close();
            break;
        }
        default:
            break;
    }
}

VertexArray* UIScreen::getSpriteVerts() const
{
    return this->mSpriteVerts;
}

void UIScreen::setBindFrameBuffer(const unsigned int& buffer)
{
    this->mBindFrameBuffer = buffer;
}

void UIScreen::setBindTexture(class Texture* tex)
{
    this->mBindTexture = tex;
}

unsigned int UIScreen::getBindFrameBuffer() const
{
    return this->mBindFrameBuffer;
}

class Texture* UIScreen::getBindTexture() const
{
    return this->mBindTexture;
}

void UIScreen::setUIPosOffset(const Vector2& offset)
{
    this->mUIPosOffset = offset;
}

Vector2 UIScreen::getUIPosOffset() const
{
    return this->mUIPosOffset;
}

void UIScreen::setUIViewScale(const Vector2& scale)
{
    this->mUIViewScale = scale;

    float w = this->getGame()->getRenderer()->getScreenWidth();
    float h = this->getGame()->getRenderer()->getScreenHeight();
    
    float sw = w * scale.x;
    float sh = h * scale.y;

    this->mUIBufferViewSize = Vector2{sw, sh};
    this->mUIBufferPos = Vector2{(w - sw) * 0.5, (h - sh) * 0.5};
}

Vector2 UIScreen::getUIViewScale() const
{
    return this->mUIViewScale;
}

void UIScreen::setUIBGColor(const Vector3& color)
{
    this->mUIBGColor = color;
}

Vector3 UIScreen::getUIBGColor() const
{
    return this->mUIBGColor;
}

void UIScreen::setUIBufferPos(const Vector2& pos)
{
    this->mUIBufferPos = pos;
}

Vector2 UIScreen::getUIBufferPos() const
{
    return this->mUIBufferPos;
}

void UIScreen::setUIBufferViewSize(const Vector2& size)
{
    this->mUIBufferViewSize = size;
}

Vector2 UIScreen::getUIBufferViewSize() const
{
    return this->mUIBufferViewSize;
}

void UIScreen::setUITranslation(const Vector2& trans)
{
    this->mUITranslation = trans;
}

Vector2 UIScreen::getUITranslation() const
{
    return this->mUITranslation;
}

Vector2 UIScreen::mousePosRemapping(const Vector2& pos)
{
    return pos - this->mUITranslation;
}

void UIScreen::setPosOffsetCoeff(const float& coeff)
{
    this->mPosOffsetCoeff = coeff;
}

float UIScreen::getPosOffsetCoeff() const
{
    return this->mPosOffsetCoeff;
}

template Button* UIScreen::addButton<Button>(const std::string& name, const std::string& type, const std::map<std::string, 
    std::string>& bindTex, const Vector2& pos, const UIBindEvent& bind_event, 
    std::function<void()> onClick, const bool& addIn, const Vector2& scale);

template Button* UIScreen::addButton<Button>(
    const std::string& name, const std::string& type, 
    const std::map<std::string, std::string>& bindTex,
    const std::wstring& text, const Vector3& color, const int& fontSize,
    const Vector2& pos, const UIBindEvent& bind_event, 
    std::function<void()> onClick, const bool& addIn, const Vector2& scale);

template TextButton* UIScreen::addButton<TextButton>(
    const std::string& name, const std::string& type, 
    const std::map<std::string, std::string>& bindTex,
    const std::wstring& text, const Vector3& color, const int& fontSize,
    const Vector2& pos, const UIBindEvent& bind_event, 
    std::function<void()> onClick, const bool& addIn, const Vector2& scale);