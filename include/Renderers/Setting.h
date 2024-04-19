/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:38:25
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 20:19:48
 * @FilePath: /CustomGame-dev/include/Renderers/Setting.h
 * @Description: Custom game engine constructing...
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#pragma once

#include "./UIScreen.h"
#include <string>
#include <stdlib.h>
#include <wchar.h>

class Setting : public UIScreen
{
private:
    float mSliderCoordMax;
    float mSliderCoordMin;

public:
    Setting(class Game* game);
    ~Setting();

    void init();

    void setGameVolume();

    // void draw(class Shader* spriteShader, class Shader* textShader, class EmptySprite* elem = nullptr, const Vector2& = Vector2{0.0f, 0.0f}) override;
    
    void update(float dt) override;
    void handleKeyPress(int key) override;
    void bindEvent(const UIBindEvent& event, class Button* b) override;
    void draw(
        class Shader* basicShader, 
        class Shader* spriteShader, 
        class Shader* fontShader,
        class EmptySprite* elem
    ) override;
};