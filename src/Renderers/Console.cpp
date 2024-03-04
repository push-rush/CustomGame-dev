#include "../../include/Renderers/Console.h"

#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/PropertyMenu.h"
#include "../../include/Renderers/Button.h"

#include "../../include/Game.h"

Console::Console(class Game* game)
: UIScreen(game)
{
    this->setUIType(EConsole);
}

Console::~Console()
{

}

void Console::draw(class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader, class EmptySprite* elem)
{
    std::sort(this->getUIElements().begin(), this->getUIElements().end(), [](EmptySprite* e1, EmptySprite* e2)
    {
        return e1->getUpdateOrder() < e2->getUpdateOrder();
    });

    for (auto e : this->getUIElements())
    {
        if (!strcmp(e->getSpriteName().c_str(), "up_border") ||
            !strcmp(e->getSpriteName().c_str(), "right_border")
        )
        {
            Button temp = *((Button*)(e));
            temp.setPosition(temp.getPosition() - this->getUITranslation());
            UIScreen::draw(basicShader, spriteShader, fontShader, (EmptySprite*)(&temp)); 
        }
        else
        {
            UIScreen::draw(basicShader, spriteShader, fontShader, e);
        }

        // SDL_Log("[Console] sprite name: %s", e->getSpriteName().c_str());
    }
}

void Console::handleKeyPress(int key)
{
    switch (key)
    {
        case (SDL_BUTTON_LEFT):
        {
            break;
        }
        case (SDL_BUTTON_RIGHT):
        {
            SDL_Log("[ResourceMenu] Left Button is down...");

            break;
        }
        case (SDLK_ESCAPE):
        {
            this->setUIState(EInvisible);
            break;
        }
        case (SDLK_SPACE):
        {
            // 设置为运行态
            this->getGame()->setGameState(Game::EGamePlay);

            this->setUIState(EInvisible);

            // 设置鼠标为相对模式
            this->setRelativeMouseMode(true);
            
            break;
        }
        // case SDL_SCANCODE_C:
        // case SDLK_c:
        // {
        //     // 设置为运行态
        //     this->getGame()->setGameState(Game::EGamePlay);

        //     this->setUIState(EInvisible);

        //     // 设置鼠标为相对模式
        //     this->setRelativeMouseMode(true);
            
        //     break;
        // }
        default:
            break;
    }
}

void Console::update(float dt)
{
    auto w = this->getGame()->getRenderer()->getScreenWidth();
    auto h = this->getGame()->getRenderer()->getScreenHeight();
    
    UIScreen::update(dt);

    Button *up_border = nullptr, *right_border = nullptr;
    for (auto elem : this->getUIElements())
    {
        if (!strcmp(elem->getSpriteName().c_str(), "up_border"))
        {   
            up_border = (Button*)(elem);
        }
        else if (!strcmp(elem->getSpriteName().c_str(), "right_border"))
        {
            right_border = (Button*)(elem);
        }
    }
    
    auto scale = this->getGame()->getPropertyMenu()->getUIViewScale();
    this->setUIViewScale(Vector2{1.0f - scale.x, this->getUIViewScale().y});
    this->setUIPosOffset(Vector2{
        -this->getUIBufferPos().x,
        -this->getUIBufferPos().y
    });

    up_border->setPosition(Vector2{
        this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + this->getUIBufferViewSize().x * 0.5f, 
        this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y
    });
    up_border->setDimension(Vector2{this->getUIBufferViewSize().x, 20.0f});

    right_border->setPosition(Vector2{
        this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + this->getUIBufferViewSize().x, 
        this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y * 0.5f
    });
    right_border->setDimension(Vector2{20.0f, this->getUIBufferViewSize().y});

    int x = 0, y = 0;
    Uint32 mouse = SDL_GetMouseState(&x, &y);
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    Vector2 relative_pos = Vector2{mousePos.x - (this->getGame()->getRenderer()->getScreenWidth() * 0.5f),
        -mousePos.y + (this->getGame()->getRenderer()->getScreenHeight() * 0.5f)
    };
    if ((mouse & SDL_BUTTON(SDL_BUTTON_LEFT)))
    {
        bool flag = false;

        if (abs(int(relative_pos.y - up_border->getPosition().y)) < 20.0f && 
            relative_pos.x < (this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + this->getUIBufferViewSize().x)
        )
        {
            Vector2 scale = Vector2{this->getUIViewScale().x, (h - mousePos.y) / h};
            this->setUIViewScale(scale);
            up_border->setPosition(Vector2{up_border->getPosition().x, relative_pos.y});

            flag = true;

            SDL_Log("[Console] scale: (%.3f, %.3f)", scale.x, scale.y);
        }
        else if ((abs(int(relative_pos.x - right_border->getPosition().x)) < 20.0f && 
            relative_pos.y > (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f)) 
        )
        {
            auto dy = relative_pos.y;

            if ((dy + h * 0.5f + right_border->getDimension().y * 0.5) > (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y))
            {
                dy = (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y) - h * 0.5f - right_border->getDimension().y * 0.5f;
            }
            else if ((dy + h * 0.5f - right_border->getDimension().y * 0.5f) < (this->getUIBufferPos().y + this->getUIPosOffset().y))
            {
                dy = (this->getUIBufferPos().y + this->getUIPosOffset().y) - h * 0.5f + right_border->getDimension().y * 0.5f;
            }

            right_border->setPosition(Vector2{right_border->getPosition().x, dy});

            this->setUITranslation(Vector2{this->getUITranslation().x, 
                this->getPosOffsetCoeff() * (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y - (dy + right_border->getDimension().y * 0.5f))
            });

            // SDL_Log("[Console] dy: %.1f pos: (%.1f, %.1f)", dy, right_border->getPosition().x, right_border->getPosition().y);
        }

        this->setUIPosOffset(Vector2{
            -this->getUIBufferPos().x,
            -this->getUIBufferPos().y
        });
    }
}

void Console::processInput(const uint8_t* keys)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
    mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

    if (!this->getUIElements().empty())
    {
        for (auto elem : this->getUIElements())
        {
            if (!strcmp(elem->getType().c_str(), "button"))
            {
                Button* b = reinterpret_cast<Button*>(elem);
                if (b->containsPoint(mousePos))
                    b->setHighlighted(true);
                else
                    b->setHighlighted(false);
            }
        }
    }

    mousePos = this->mousePosRemapping(mousePos);
}

void Console::handleMouseWheel(const int& mouse_wheel)
{
    // SDL_Log("[Console] Mouse wheel is scrolling...");

    UIScreen::handleMouseWheel(mouse_wheel);

    int x = 0, y = 0;
    Uint32 mouse = SDL_GetMouseState(&x, &y);
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    Vector2 relative_pos = Vector2{mousePos.x - (this->getGame()->getRenderer()->getScreenWidth() * 0.5f),
        -mousePos.y + (this->getGame()->getRenderer()->getScreenHeight() * 0.5f)
    };

    float w = this->getGame()->getRenderer()->getScreenWidth();
    float h = this->getGame()->getRenderer()->getScreenHeight();

    if (mouse_wheel && 
        (relative_pos.y < (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y)) &&
        (relative_pos.x < (this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + this->getUIBufferViewSize().x))    
    )
    {
        Button *up_border = nullptr, *right_border = nullptr;
        for (auto elem : this->getUIElements())
        {
            if (!strcmp(elem->getSpriteName().c_str(), "up_border"))
            {
                up_border = (Button*)(elem);
            }
            else if (!strcmp(elem->getSpriteName().c_str(), "right_border"))
            {
                right_border = (Button*)(elem);
            }
        }
        
        auto dy = right_border->getPosition().y + mouse_wheel * 10.0f;
        SDL_Log("[Console] Mouse wheel is scrolling: y: %d %.1f", mouse_wheel, dy);

        if ((dy + h * 0.5f + right_border->getDimension().y * 0.5) > (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y))
        {
            dy = (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y) - h * 0.5f - right_border->getDimension().y * 0.5f;
        }
        else if ((dy + h * 0.5f - right_border->getDimension().y * 0.5f) < (this->getUIBufferPos().y + this->getUIPosOffset().y))
        {
            dy = (this->getUIBufferPos().y + this->getUIPosOffset().y) - h * 0.5f + right_border->getDimension().y * 0.5f;
        }

        right_border->setPosition(Vector2{right_border->getPosition().x, dy});

        this->setUITranslation(Vector2{this->getUITranslation().x, 
            this->getPosOffsetCoeff() * (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y - (dy + right_border->getDimension().y * 0.5f))
        });
    }
}