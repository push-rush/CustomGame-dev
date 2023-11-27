#pragma once

#include "./Actor.h"

class TargetActor : public Actor
{
private:
    class TargetComponent* mTargetComponent;
    class MeshComponent* mMeshComponent;
    class BoxComponent* mBoxComponent;
    
public:
    TargetActor(class Game* game);
    ~TargetActor();

    Actor::EActorType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};