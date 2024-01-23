#include "../../include/Renderers/TextButton.h"

#include "../../include/Renderers/UIScreen.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/Graphics2d.h"
#include "../../include/Renderers/FreeTypeFont.h"

#include "../../include/Game.h"

#include <SDL.h>

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
    this->mBoxColor = Vector3{0.85f, 0.85f, 0.85f};
    this->mBGColor = Vector3{0.25f, 0.25f, 0.25f};

    for (auto tex : this->getNameTextures())
    {
        this->mModTextTextures.emplace_back(tex);
    }
    this->mModTextSize = this->getTextSize();

    this->mCurTimestamp = SDL_GetTicks();
}

TextButton::~TextButton()
{

}

void TextButton::Rename(class SDL_Window* window, class Shader* spriteShader, class Shader* fontShader)
{
    SDL_Log("[TextButton] Rename...");
    bool inputting = true;

    SDL_Event event;
    Vector2 mousePos = Vector2{0.0f, 0.0f};
    
    // 设置帧缓存区索引为0
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 禁用深度缓存区
    glDisable(GL_DEPTH_TEST);

    // 开启Alpha混合渲染
    // glEnable(GL_BLEND);

    while (inputting)
    {
        while (SDL_PollEvent(&event))
        {
            SDL_Log("[TextButton] key type: %d", event.type);
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                mousePos = Vector2{(float)event.button.x, (float)event.button.y};
                mousePos.x -= this->getUIScreen()->getGame()->getRenderer()->getScreenWidth() * 0.5f;
                mousePos.y = this->getUIScreen()->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

                if (this->containsPoint(mousePos))
                {   // 鼠标点击文本框外，结束文本输入
                    inputting = false;
                    break;
                }
            }
            else if (event.type == SDL_KEYDOWN)
            {
                std::wstring text = this->mModText;
                int len = text.length();
                switch (event.key.keysym.sym)
                {
                    case SDLK_BACKSPACE:
                    {   // 输入退格键，删掉一个字符
                        if (len > 0)
                        {
                            wchar_t wch = text[len-1];
                            text[len - 1] = 0;
                            this->mModText = text;
                            auto iter = this->mModTextTextures.erase(this->mModTextTextures.end() - 1); 
                            Character* ct = this->getUIScreen()->getGame()->getFreeTypeFont()->renderFont(wch);
                            this->mModTextSize.x -= (ct->Advance >> 6);
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
                            text[len++] = event.key.keysym.sym;
                            text[len] = 0;
                            this->mModText = text;
                            
                            Character* ch = this->getUIScreen()->getGame()->getFreeTypeFont()->renderFont(event.key.keysym.sym);
                            class Texture* tex = this->getUIScreen()->getGame()->getFreeTypeFont()->getFontTexture(ch, Vector2{this->mModTextSize.x, 0.0f});
                            this->mModTextTextures.emplace_back(tex);
                            this->mModTextSize.x += (ch->Advance >> 6);
                        }
                        break;   
                    }
                }
            }
        }

        // 修改背景色
        Renderers::Graphics2d::drawRectangle(
            spriteShader, 
            this->getPosition(), this->getDimension().x + 100, this->getDimension().y, 
            this->mBGColor
        );

        // 添加矩形边框
        std::vector<Vector2> points;
        points.emplace_back(Vector2{this->getPosition().x - this->getDimension().x * 0.5, this->getPosition().y + this->getDimension().y * 0.5});
        points.emplace_back(Vector2{this->getPosition().x + this->getDimension().x * 0.5, this->getPosition().y + this->getDimension().y * 0.5});
        points.emplace_back(Vector2{this->getPosition().x + this->getDimension().x * 0.5, this->getPosition().y - this->getDimension().y * 0.5});
        points.emplace_back(Vector2{this->getPosition().x - this->getDimension().x * 0.5, this->getPosition().y - this->getDimension().y * 0.5});
        Renderers::Graphics2d::drawPolygon(
            spriteShader,
            points,
            this->mBoxColor,
            3
        );

        // 修改文本
        float dx = -0.5 * this->getTextSize().x;
        float dy = 0.5 * this->getTextSize().y;
        this->getUIScreen()->drawTextures(fontShader, this->mModTextTextures, 
            Vector2{this->getPosition().x + dx, this->getPosition().y - dy}, 
            1.0f, 1.0f, false, true
        );

        this->mCurTimestamp = SDL_GetTicks();
        if (this->mCurTimestamp % 1000 <= 500)
        {
            // 绘制光标
            Renderers::Graphics2d::drawLine(
                spriteShader,
                this->getPosition() + Vector2{dx + this->mModTextSize.x * 0.5, -dy},
                this->getPosition() + Vector2{dx + this->mModTextSize.x * 0.5, dy},
                Color::White,
                2
            );
        }
        SDL_GL_SwapWindow(window);
    }
}

void TextButton::setBgColor(const Vector3& color)
{
    this->mBGColor = color;
}

void TextButton::setBoxColor(const Vector3& color)
{
    this->mBoxColor = color;
}

void TextButton::setTextLength(const int& len)
{
    this->mMaxTextLength = len;
}
