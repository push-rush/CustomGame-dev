#pragma once

#include "./UIScreen.h"
#include <string>
#include <stdlib.h>
#include <wchar.h>

class Setting : public UIScreen
{
private:
    // class Texture* mSoundSlider;
    // class Texture* mSoundSliderOn;
    // class Texture* mSoundSliderOff;

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