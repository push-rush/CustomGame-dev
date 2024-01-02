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
    void handleKeyPress(int key);
    void setGameVolume();
    void bindEvent(const UIBindEvent& event);
    void draw(class Shader* spriteShader, class Shader* textShader, class EmptySprite* elem = nullptr) override;
};