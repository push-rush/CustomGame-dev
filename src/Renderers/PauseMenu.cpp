#include "../../include/Game.h"
#include "../../include/Renderers/PauseMenu.h"
#include "../../include/Renderers/DialogBox.h"

PauseMenu::PauseMenu(class Game* game)
: UIScreen(game)
{
    this->setUIType(UIScreen::EPauseMenu);

    // 在构造函数中将游戏状态设置为“暂停”状态
    this->getGame()->setGameState(Game::EPaused);
    this->setTitleTexture(L"游戏菜单", Vector3{0.8f, 0.35f, 0.35f});

    this->addButton(RectButtonON, L"回到游戏", this->getTextColor(), [this](){
            this->close();
            std::cout << "[PauseMenu] Close..." << std::endl;
        }, Vector2{1.35f, 1.0f}
    );
    
    this->addButton(RectButtonON, L"退出游戏", this->getTextColor(), [this](){
        new DialogBox(this->getGame(), L"退出菜单",
            [this](){
                this->getGame()->setGameState(Game::EQuit);
                std::cout << "dialog box is quit" << std::endl;
            }
        );}, Vector2{1.35, 1.0f}
    );
    
    // 暂停时禁用相对鼠标模式
    this->setRelativeMouseMode(false);
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

    if (key == SDLK_ESCAPE)
    {
        close();
    }
}