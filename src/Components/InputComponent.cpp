#include "../../include/Components/InputComponent.h"
#include "../../include/Actors/Actor.h"
#include "../../include/General/LevelLoader.h"

InputComponent::InputComponent(/* args */)
{

}

InputComponent::InputComponent(class Actor* owner, int updateOrder)
: MoveComponent(owner)
{
    this->mForwardKey = 0;
    this->mBackKey = 0;
    this->mClockwiseKey = 0;
    this->mCounterClockwiseKey = 0;
    this->mMaxForwardSpeed = 568.0f;
    this->mMaxAngularSpeed = Math::TwoPi;
}

InputComponent::~InputComponent()
{

}

void InputComponent::processInput(const uint8_t* keyboard_state)
{
    float forwardSpeed = 0.0f;

    if (keyboard_state[mForwardKey])
    {
        forwardSpeed += mMaxForwardSpeed;
    }
    if (keyboard_state[mBackKey])
    {
        forwardSpeed -= mMaxForwardSpeed;
    }
    setForwardSpeed(forwardSpeed);

    float angularSpeed = 0.0f;
    if (keyboard_state[mClockwiseKey])
    {
        angularSpeed -= mMaxAngularSpeed;
    }
    if (keyboard_state[mCounterClockwiseKey])
    {
        angularSpeed += mMaxAngularSpeed;
    }
    setAngularSpeed(angularSpeed);
    this->getActor()->recomputeWorldTransform(true);
}

void InputComponent::setForwardKey(uint8_t key)
{
    this->mForwardKey = key;
}

void InputComponent::setBackKey(uint8_t key)
{
    this->mBackKey = key;
}

void InputComponent::setClockwiseKey(uint8_t key)
{
    this->mClockwiseKey = key;
}

void InputComponent::setCounterClockwiseKey(uint8_t key)
{
    this->mCounterClockwiseKey = key;
}

void InputComponent::setMaxForwardSpeed(float max_speed)
{
    this->mMaxForwardSpeed = max_speed;
}

void InputComponent::setMaxAngularSpeed(float max_speed)
{
    this->mMaxAngularSpeed = max_speed;
}

Component::EComponentType InputComponent::getType() const
{
    return EInputComponent;
}

void InputComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void InputComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}