#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

#include "./Actor.h"

class OrbitActor : public Actor
{
private:
    class OrbitCamera* mCameraComponent;
    class MeshComponent* mMeshComponent;
    class MoveComponent* mMoveComponent;

public:
    OrbitActor(class Game* game);
    ~OrbitActor();

    void actorInput(const uint8_t* keyboard_state) override;
    Actor::EActorType getType() const override;
    
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};