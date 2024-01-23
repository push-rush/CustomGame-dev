#pragma once

#include "./UIScreen.h"

class Console : public UIScreen
{
private:
    
public:
    Console(class Game* game);
    ~Console();

    // 处理按键输入
    void handleKeyPress(int key) override;

    // 外部输入处理
    void processInput(const uint8_t* keys) override;

    // 更新ui
    void update(float dt) override;

    void handleMouseWheel(const int& mouse_wheel) override;

    void draw(
        class Shader* basicShader, 
        class Shader* spriteShader, 
        class Shader* fontShader,
        class EmptySprite* elem
    ) override;
};