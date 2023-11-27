#pragma once 

#include "./Actor.h"

class Laser : public Actor
{
private:
    float mDeathTime;
    class CircleComponent* mCircleComponent;
    class SpriteComponent* mSpriteComponent;
    
public:
    Laser(/* args */);
    Laser(class Game* game);
    ~Laser();

    void updateActor(float dt) override;
    SpriteComponent* getSpriteComponent() const override;

    Actor::EActorType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
