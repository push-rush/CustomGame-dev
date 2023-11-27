#include "../../include/Components/MoveComponent.h"
#include "../../include/Actors/Actor.h"
#include "../../include/General/LevelLoader.h"

#include "SDL.h"

MoveComponent::MoveComponent()
{
    
}

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
: Component(owner, updateOrder)
{
    this->mForwardSpeed = 0.0f;
    this->mAngularSpeed = 0.0f;
    this->mStrafeSpeed = 0.0f;
    this->mPitchSpeed = 0.0f;
}

MoveComponent::~MoveComponent()
{
    
}

void MoveComponent::update(float dt)
{
    // 更新旋转位置
    if (!Math::NearZero(mAngularSpeed))
    {
        Quaternion quat = this->getActor()->getRotation();
        
        // 绕z轴旋转
        float angle = mAngularSpeed * dt;
        Quaternion inc(Vector3::UnitZ, angle);

        // 将两个四元数合并得到新的结果
        quat = Quaternion::Concatenate(quat, inc);
        this->getActor()->setRotation(quat);       
    }

    // 更新俯仰位置
    // if (!Math::NearZero(this->mPitchSpeed))
    // {
    //     Quaternion quat = this->getActor()->getRotation();

    //     // 绕y轴旋转
    //     float angle = this->mPitchSpeed * dt;
    //     Quaternion inc(Vector3::UnitY, angle);

    //     // 两个旋转合并
    //     quat = Quaternion::Concatenate(quat, inc);
    //     this->getActor()->setRotation(quat);

    //     SDL_Log("pitchSpeed: %f", this->mPitchSpeed);
    // }

    // 更新前行位置
    if (!Math::NearZero(mForwardSpeed))
    {
        // Vector2 pos = this->getActor()->getPosition();
        Vector3 pos = this->getActor()->getPosition();
        pos += this->getActor()->getForward() * this->mForwardSpeed * dt;
        // pos.z = 0.f;
        this->getActor()->setPosition(pos);
    }

    // 更新侧移位置
    if (!Math::NearZero(this->mStrafeSpeed))
    {
        Vector3 pos = this->getActor()->getPosition();
        pos += this->getActor()->getRight() * this->mStrafeSpeed * dt;
        this->getActor()->setPosition(pos);
    }

    this->getActor()->recomputeWorldTransform(true);
}

float MoveComponent::getAngularSpeed() const
{
    return this->mAngularSpeed;
}   

float MoveComponent::getForwardSpeed() const
{
    return this->mForwardSpeed;
}

void MoveComponent::setAngularSpeed(float angularSpeed)
{
    this->mAngularSpeed = angularSpeed;
}

void MoveComponent::setForwardSpeed(float forwardSpeed)
{
    this->mForwardSpeed = forwardSpeed;
}

void MoveComponent::setStrafeSpeed(float strafeSpeed)
{
    this->mStrafeSpeed = strafeSpeed;
}

void MoveComponent::setPitchSpeed(float pitchSpeed)
{
    this->mPitchSpeed = pitchSpeed;
}

Component::EComponentType MoveComponent::getType() const
{
    return EMoveComponent;
}

void MoveComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void MoveComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}