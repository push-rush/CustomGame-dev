#include "../../include/Renderers/DialogBox.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/EmptySprite.h"

DialogBox::DialogBox(class Game* game)
: UIScreen(game)
{
    this->setUIType(UIScreen::EDialogBox);
}

DialogBox::DialogBox(Game* game, const std::wstring& text, std::function<void()> onOK)
: UIScreen(game)
{
    this->setUIType(UIScreen::EDialogBox);

    this->setBGPos(Vector2(0.0f, 0.0f));
    this->setTitlePos(Vector2(0.0f, 100.0f));
    
    // this->setNextButtonPos(Vector2(0.0f, 0.0f));

    // 设置背景
    // this->addBackground(game->getRenderer()->getTexture("../Assets/Images/DialogBG.png"));
    // this->setTitleTexture(text, Vector3::Zero, 40);

    // 设置按钮
    // this->addButton(RectButtonON, L"确定", this->getTextColor(), [onOK](){
    //     onOK();
    // });

    // this->addButton(RectButtonON, L"取消", this->getTextColor(), [this](){
    //     this->close();
    // });

    std::cout << "dialog ..." << std::endl;
}

DialogBox::~DialogBox()
{
    // this->getGame()->setGameState(Game::EGamePlay);
    // this->setRelativeMouseMode(true);

    std::cout << "[DialogBox] Releasing..." << std::endl;
}

void DialogBox::close()
{
    // this->setUIState(EClosing);
    this->setUIState(EInvisible);

    auto s = this->getGame()->getUIStack();
    for (auto ui : s)
    {
        if (ui->getUIType() == EPauseMenu)
        {
            ui->setUIState(EActiviting);
        }
    }
}

// void DialogBox::draw(class Shader* spriteShader, class Shader* fontShader, EmptySprite* elem, const Vector2&)
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

void DialogBox::draw(class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader, class EmptySprite* elem)
{
    std::sort(this->getUIElements().begin(), this->getUIElements().end(), [](EmptySprite* e1, EmptySprite* e2)
    {
        return e1->getUpdateOrder() < e2->getUpdateOrder();
    });

    for (auto e : this->getUIElements())
    {
        UIScreen::draw(basicShader, spriteShader, fontShader, e);
    }
}

void DialogBox::handleKeyPress(int key)
{
    UIScreen::handleKeyPress(key);


}