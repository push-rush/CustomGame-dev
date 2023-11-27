#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "./Actor.h"
#include "../include/Audios/SoundEvent.h"

class FPSActor : public Actor
{
private:
    SoundEvent mFootstep;
    class MoveComponent* mMoveComponent;
    class FPSCamera* mCameraComponent;
    class AudioComponent* mAudioComponent;
    float mFootstepCooldown;
    class MeshComponent* mMeshComponent;
    class Actor* mFPSModel;
    class BoxComponent* mBoxComponent;

public:
    FPSActor(class Game* game);
    ~FPSActor();

    void actorInput(const uint8_t* keyboard_state) override;
    void updateActor(float dt) override;
    void setFootstepSurface(float);

    // 最小重叠碰撞测试
    void fixCollisions();

    // 发射弹球
    void shoot();

    Actor::EActorType getType() const override;
    
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};