#include "../../include/Renderers/DialogBox.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Texture.h"

DialogBox::DialogBox(Game* game, const std::wstring& text, std::function<void()> onOK)
: UIScreen(game)
{
    this->setUIType(UIScreen::EDialogBox);

    this->setBGPos(Vector2(0.0f, 0.0f));
    this->setTitlePos(Vector2(0.0f, 100.0f));
    this->setNextButtonPos(Vector2(0.0f, 0.0f));

    // 设置背景
    this->addBackground(game->getRenderer()->getTexture("../Assets/Images/DialogBG.png"));
    this->setTitleTexture(text, Vector3::Zero, 40);

    // 设置按钮
    this->addButton(RectButtonON, L"确定", this->getTextColor(), [onOK](){
        onOK();
    });

    this->addButton(RectButtonON, L"取消", this->getTextColor(), [this](){
        this->close();
    });

    std::cout << "dialog ..." << std::endl;
}

DialogBox::~DialogBox()
{
    // this->getGame()->setGameState(Game::EGamePlay);
    // this->setRelativeMouseMode(true);

    std::cout << "[DialogBox] Releasing..." << std::endl;
}