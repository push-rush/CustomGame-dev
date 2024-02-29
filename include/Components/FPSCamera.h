#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "./CameraComponent.h"

class FPSCamera : public CameraComponent
{
private:
    float mPitchSpeed;
    float mMaxPitch;
    float mPitch;

    Vector3 mPosOffset;

public:
    FPSCamera(class Actor* owner);
    ~FPSCamera();

    void update(float dt) override;
    void processInput(const uint8_t* keyboard_state) override;
    void handleMouseWheel(const int& mouse_wheel) override;

    void setPitchSpeed(float pitchSpeed);    
    void setPitch(float pitch);
    void setMaxPitch(float maxPitch);
    void setPosOffset(const Vector3& offset);

    float getPitchSpeed() const;
    float getMaxPitch() const;
    float getPitch() const;
    Vector3 getPosOffset() const;

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
