#pragma once

#include "./UIScreen.h"

class PauseMenu : public UIScreen
{
public:
    PauseMenu(class Game* game);
    ~PauseMenu();

    void init();
    void handleKeyPress(int key);
    void draw(class Shader* spriteShader, class Shader* textShader, class EmptySprite* elem = nullptr) override;
    void bindEvent(const UIBindEvent& event) override;

private:
    bool mUpdated = false;
    
};