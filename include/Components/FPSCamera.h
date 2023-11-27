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

public:
    FPSCamera(class Actor* owner);
    ~FPSCamera();

    void update(float dt) override;

    void setPitchSpeed(float pitchSpeed);    
    void setPitch(float pitch);
    void setMaxPitch(float maxPitch);

    float getPitchSpeed() const;
    float getMaxPitch() const;
    float getPitch() const;

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
