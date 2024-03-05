#include "../../include/Renderers/PropertyMenu.h"

#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/ResourceMenu.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Button.h"

#include "../../include/Game.h"

PropertyMenu::PropertyMenu(class Game* game)
: UIScreen(game)
{
    this->setUIType(EPropertyMenu);
}

PropertyMenu::~PropertyMenu()
{

}

void PropertyMenu::draw(class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader, class EmptySprite* elem)
{
    std::sort(this->getUIElements().begin(), this->getUIElements().end(), [](EmptySprite* e1, EmptySprite* e2)
    {
        return e1->getUpdateOrder() < e2->getUpdateOrder();
    });

    for (auto e : this->getUIElements())
    {
        if (!strcmp(e->getSpriteName().c_str(), "right_border"))
        {
            Button temp = *((Button*)(e));
            temp.setPosition(temp.getPosition() - this->getUITranslation());
            UIScreen::draw(basicShader, spriteShader, fontShader, (EmptySprite*)(&temp)); 
        }
        else
        {
            UIScreen::draw(basicShader, spriteShader, fontShader, e);
        }
    }
}

void PropertyMenu::handleKeyPress(int key)
{
    switch (key)
    {
    case (SDL_BUTTON_LEFT):
    {
        SDL_Log("[PropertyMenu] Left Button is down...");

        break;
    }
    case (SDL_BUTTON_RIGHT):
    {
        SDL_Log("[PropertyMenu] Right Button is down...");

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
    default:
        break;
    }
}

void PropertyMenu::update(float dt)
{
    UIScreen::update(dt);

    Button *right_border = nullptr;
    for (auto elem : this->getUIElements())
    {
        if (!strcmp(elem->getSpriteName().c_str(), "right_border"))
        {
            right_border = (Button*)(elem);
        }
    }

    auto w = this->getGame()->getRenderer()->getScreenWidth();
    auto h = this->getGame()->getRenderer()->getScreenHeight();

    auto scale = this->getGame()->getResourceMenu()->getUIViewScale();
    auto old_scale = this->getUIViewScale();
    if (!Math::NearZero(scale.x - old_scale.x) || !Math::NearZero(1.0f - scale.y - old_scale.y))
    {
        this->setUIViewScale(Vector2{scale.x, 1.0f - scale.y});
        this->setUIPosOffset(Vector2{
            w - this->getUIBufferPos().x - this->getUIBufferViewSize().x, 
            -this->getUIBufferPos().y
        });

        right_border->setPosition(Vector2{
            this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + this->getUIBufferViewSize().x, 
            this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y * 0.5f
        });
        right_border->setDimension(Vector2{20.0f, this->getUIBufferViewSize().y});

        EmptySprite* left_bar = nullptr;
        for (auto elem : this->getUIElements())
        {
            if (!strcmp(elem->getSpriteName().c_str(), "Sidebar"))
            {
                left_bar = elem;
                break;
            }
        }

        left_bar->setDimension(Vector2{left_bar->getDimension().x, this->getUIBufferViewSize().y});
        left_bar->setPosition(Vector2{
            this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + left_bar->getDimension().x * 0.5f,
            this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + left_bar->getDimension().y * 0.5f
        });
    }

    int x = 0, y = 0;
    Uint32 mouse = SDL_GetMouseState(&x, &y);
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    Vector2 relative_pos = Vector2{mousePos.x - (this->getGame()->getRenderer()->getScreenWidth() * 0.5f),
        -mousePos.y + (this->getGame()->getRenderer()->getScreenHeight() * 0.5f)
    };
    if ((mouse & SDL_BUTTON(SDL_BUTTON_LEFT)))
    {
        bool flag = false;

        if ((abs(int(relative_pos.x - right_border->getPosition().x)) < 20.0f && 
            relative_pos.y < (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y)) 
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
            w - this->getUIBufferPos().x - this->getUIBufferViewSize().x, 
            -this->getUIBufferPos().y
        });
    }
}

void PropertyMenu::handleMouseWheel(const int& mouse_wheel)
{
    // SDL_Log("[PropertyMenu] Mouse wheel is scrolling...");

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
        (relative_pos.x > (this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f))
    )
    {
        Button *right_border = nullptr;
        for (auto elem : this->getUIElements())
        {
            if (!strcmp(elem->getSpriteName().c_str(), "right_border"))
            {
                right_border = (Button*)(elem);
            }
        }
        
        auto dy = right_border->getPosition().y + mouse_wheel * 10.0f;

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

        SDL_Log("[PropertyMenu] Mouse wheel is scrolling: y: %d %.1f", mouse_wheel, dy);
    }

}

void PropertyMenu::processInput(const uint8_t* keys)
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