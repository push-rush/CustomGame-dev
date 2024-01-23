#include "../../include/Game.h"
#include "../../include/Renderers/PauseMenu.h"
#include "../../include/Renderers/DialogBox.h"
#include "../../include/Renderers/EmptySprite.h"


PauseMenu::PauseMenu(class Game* game)
: UIScreen(game)
{
    this->setUIType(UIScreen::EPauseMenu);

    // 在构造函数中将游戏状态设置为“暂停”状态
    // this->getGame()->setGameState(Game::EPaused);
    // this->setTitleTexture(L"游戏菜单", Vector3{0.8f, 0.35f, 0.35f});

    // this->addButton( 
    //     "button1", "button", {
    //         {"off", "button1"},
    //         {"on", "button2"}
    //     },
    //     L"回到游戏", this->getTextColor(), 
    //     Vector2{0.0f, 120.0f}, 
    //     [this](){
    //         this->close();
    //         std::cout << "[PauseMenu] Close..." << std::endl;
    //     }, Vector2{1.35f, 1.0f}
    // );
    
    // this->addButton(
    //     "button2", "button", {
    //         {"off", "button1"},
    //         {"on", "button2"}
    //     },
    //     L"退出游戏", this->getTextColor(), 
    //     Vector2{0.0f, 150.0f},
    //     [this](){
    //         // 添加dialog界面
    //         this->getGame()->pushUI(this->getGame()->getDialogBox());
    //     }, Vector2{1.35, 1.0f}
    // );
    
    // 暂停时禁用相对鼠标模式
    // this->setRelativeMouseMode(false);
    // SDL_SetRelativeMouseMode(SDL_FALSE);
}

PauseMenu::~PauseMenu()
{
    // 退出暂停重新启用相对鼠标模式
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    // SDL_GetRelativeMouseState(nullptr, nullptr);

    // 将游戏状态重新设置回“进行中”状态
    this->getGame()->setGameState(Game::EGamePlay);
    this->setRelativeMouseMode(true);

    SDL_Log("[PauseMenu] Releasing...");
    // system("pause");
}

void PauseMenu::handleKeyPress(int key)
{
    UIScreen::handleKeyPress(key);
    // if (key == SDLK_ESCAPE)
    // {
    //     // 退出当前ui菜单
    //     close();
    // }
}

void PauseMenu::init()
{
    if (!mUpdated)
    {
        // this->setTitleTexture(L"游戏菜单", Vector3{0.8f, 0.35f, 0.35f});
        // this->addButton( 
        //     "button1", "button", {
        //         {"off", "button1"},
        //         {"on", "button2"}
        //     },
        //     L"回到游戏", this->getTextColor(), 
        //     Vector2{0.0f, 120.0f}, 
        //     [this](){
        //         this->close();
        //         std::cout << "[PauseMenu] Close..." << std::endl;
        //     }, Vector2{1.35f, 1.0f}
        // );

        // this->addButton(
        //     "button2", "button", {
        //         {"off", "button1"},
        //         {"on", "button2"}
        //     },
        //     L"退出游戏", this->getTextColor(), 
        //     Vector2{0.0f, 150.0f},
        //     [this](){
        //         // 添加dialog界面
        //         this->getGame()->pushUI(this->getGame()->getDialogBox());
        //     }, Vector2{1.35, 1.0f}
        // );
        
        mUpdated = true;
    }
}

void PauseMenu::bindEvent(const UIBindEvent& event)
{
    UIScreen::bindEvent(event);

    switch (event)
    {
        case UIQuit:
        {
            // auto dialog_box = this->getGame()->getDialogBox();
            // dialog_box->setUIState(UIScreen::EActive);
            // this->getGame()->pushUI(dialog_box);

            auto s = this->getGame()->getUIStack();
            for (auto ui : s)
            {
                if (ui->getUIType() == EDialogBox)
                {
                    ui->setUIState(EActiviting);
                    this->setUIState(EVisible);
                    // SDL_Log("[PauseMenu] Set Dialogbox active: addr:%d type-%d state-%d", ui, ui->getUIType(), (int)ui->getUIState());
                    break;
                }
            }

            break;
        }
        default:
            break;
    }
}

// void PauseMenu::draw(class Shader* spriteShader, class Shader* fontShader, EmptySprite* elem, const Vector2&)
// {
//     std::sort(this->getUIElements().begin(), this->getUIElements().end(), [](EmptySprite* e1, EmptySprite* e2)
//     {
//         return e1->getUpdateOrder() < e2->getUpdateOrder();
//     });

//     for (auto e : this->getUIElements())
//     {
//         // SDL_Log("[PauseMenu] ui elem: %s", e->getSpriteName().c_str());
//         UIScreen::draw(spriteShader, fontShader, e);
//     }
// }

void PauseMenu::draw(class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader, class EmptySprite* elem)
{
    std::sort(this->getUIElements().begin(), this->getUIElements().end(), [](EmptySprite* e1, EmptySprite* e2)
    {
        return e1->getUpdateOrder() < e2->getUpdateOrder();
    });

    for (auto e : this->getUIElements())
    {
        // SDL_Log("[PauseMenu] ui elem: %s", e->getSpriteName().c_str());
        UIScreen::draw(basicShader, spriteShader, fontShader, e);
    }
}