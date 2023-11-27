#pragma once

#include "./UIScreen.h"
#include <string>
#include <stdlib.h>
#include <wchar.h>

class Setting : public UIScreen
{
private:
    class Texture* mSoundSlider;
    class Texture* mSoundSliderOn;
    class Texture* mSoundSliderOff;

    float mSliderCoordMax;
    float mSliderCoordMin;

public:
    Setting(class Game* game);
    ~Setting();

    void handleKeyPress(int key);
    void setGameVolume();
};