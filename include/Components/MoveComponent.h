#pragma once
#include "./Component.h"

class MoveComponent : public Component
{
public:
    MoveComponent();
    MoveComponent(class Actor* owner, int updateOrder = 10);
    ~MoveComponent();

    void update(float dt) override;

    float getAngularSpeed() const;
    float getForwardSpeed() const;
    void setAngularSpeed(float angularSpeed);
    void setForwardSpeed(float forwardSpeed);
    void setStrafeSpeed(float strafeSpeed);
    void setPitchSpeed(float pitchSpeed);

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;

private:
    float mAngularSpeed; // 旋转速度
    float mForwardSpeed; // 前进速度
    float mStrafeSpeed;  // 侧行速度
    float mPitchSpeed;   // 俯仰速度（不需要）
};