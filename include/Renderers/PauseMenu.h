#pragma once

#include "./UIScreen.h"

class PauseMenu : public UIScreen
{
public:
    PauseMenu(class Game* game);
    ~PauseMenu();

    void init();
    void handleKeyPress(int key) override;
    
    // void draw(class Shader* spriteShader, class Shader* textShader, class EmptySprite* elem = nullptr, const Vector2& = Vector2{0.0f, 0.0f}) override;
    
    void draw(
        class Shader* basicShader, 
        class Shader* spriteShader, 
        class Shader* fontShader,
        class EmptySprite* elem
    ) override;

    void bindEvent(const UIBindEvent& event, class Button* b) override;

private:
    bool mUpdated = false;
};