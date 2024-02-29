#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "./MoveComponent.h"

class InputComponent : public MoveComponent
{
private:
    float mMaxForwardSpeed;
    float mMaxAngularSpeed;
    uint8_t mForwardKey;
    uint8_t mBackKey;
    uint8_t mClockwiseKey;
    uint8_t mCounterClockwiseKey;

public:
    InputComponent(/* args */);
    InputComponent(class Actor* owner, int updateOrder = 100);
    ~InputComponent();

    void processInput(const uint8_t* keyboard_state) override;
    void handleMouseWheel(const int& mouse_wheel) override;

    void setForwardKey(uint8_t key);
    void setBackKey(uint8_t key);
    void setClockwiseKey(uint8_t key);
    void setCounterClockwiseKey(uint8_t key);
    void setMaxForwardSpeed(float);
    void setMaxAngularSpeed(float);

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
