#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "./CameraComponent.h"

class OrbitCamera : public CameraComponent
{
private:
    Vector3 mOffset;
    Vector3 mUp;
    float mPitchSpeed;
    float mYawSpeed;

public:
    OrbitCamera(class Actor* owner);
    ~OrbitCamera();

    void update(float dt);

    // Getters
    float getPitchSpeed() const;
    float getYawSpeed() const;
    Vector3 getOffset() const;
    Vector3 getUp() const;

    // Setters
    void setPitchSpeed(float);
    void setYawSpeed(float);
    void setOffset(Vector3);
    void setUp(Vector3);

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
