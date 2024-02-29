#pragma once

#include "./Actor.h"

class SplineActor : public Actor
{
private:
    class SplineCamera* mCameraComponent;

public:
    SplineActor(class Game* game);
    ~SplineActor();

    void actorInput(const uint8_t* keyboardStates) override;
    void actorHandleMouse(const int& mouse_wheel) override;
    
    Actor::EActorType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};