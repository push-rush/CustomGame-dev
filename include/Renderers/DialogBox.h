#pragma once

#include "./UIScreen.h"

// namespace Renderer2D
// {
    class DialogBox : public UIScreen
    {
    public:
        DialogBox(class Game* game);
        DialogBox(class Game* game, const std::wstring& text, std::function<void()> onOK);
        ~DialogBox();

        void draw(class Shader* spriteShader, class Shader* textShader, class EmptySprite* elem = nullptr) override;

    private:
        void close() override;
    };
// }