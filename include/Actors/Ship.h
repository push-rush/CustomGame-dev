#pragma once
#include "./Actor.h"

class Ship : public Actor
{
private:    
    class InputComponent* mInputComponent;
    class SpriteComponent* mSpriteComponent;
    class CircleComponent* mCircleComponent;
    
    float mLaserCooldown; // 冷却

public:
    Ship(class Game* game);
    ~Ship();

    void updateActor(float dt) override;
    void actorInput(const uint8_t* keyboard_state) override;
    void actorHandleMouse(const int& mouse_wheel) override;

    class SpriteComponent* getSpriteComponent() const;
    float getLaserCooldown() const;
    class CircleComponent* getCircleComponent() const;
    
    // void setPosition(class Vector2 pos) override;
    void setPosition(class Vector3 pos) override;

    Actor::EActorType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
