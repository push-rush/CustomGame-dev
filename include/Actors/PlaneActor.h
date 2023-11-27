#pragma once

#include "./Actor.h"

class PlaneActor : public Actor
{
private:
    class BoxComponent* mBoxComponent;

public:
    PlaneActor(class Game* game);
    ~PlaneActor();

    class BoxComponent* getBoxComponent();
    Actor::EActorType getType() const override;
    
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
