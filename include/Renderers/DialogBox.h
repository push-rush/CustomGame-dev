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

        void handleKeyPress(int key) override;

        // void draw(class Shader* spriteShader, class Shader* textShader, class EmptySprite* elem = nullptr, const Vector2& = Vector2{0.0f, 0.0f}) override;
        void draw(
            class Shader* basicShader, 
            class Shader* spriteShader, 
            class Shader* fontShader,
            class EmptySprite* elem
        ) override;

    private:
        void close() override;
    };
// }