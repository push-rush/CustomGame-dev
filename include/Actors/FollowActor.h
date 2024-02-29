#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>

#include "./Actors/Actor.h"
#include "./Audios/SoundEvent.h"

class FollowActor : public Actor
{
private:
    class MeshComponent* mMeshComponent;
    class MoveComponent* mMoveComponent;
    class FollowCamera* mFollowComponent;
    class BoxComponent* mBoxComponent;
    class SkeletalMeshComponent* mSkeletalComponent;
    class AudioComponent* mAudioComponent;
    class MirrorCamera* mMirrorCameraComponent;
    SoundEvent mFootstep;
    float mFootstepCooldown;
    bool mMoving;

public:
    FollowActor(class Game* game);
    ~FollowActor();

    void actorInput(const uint8_t* keyboard_state) override;
    void actorHandleMouse(const int& mouse_wheel) override;
    
    void updateActor(float dt) override;
    void setFootstepSurface(float);
    Actor::EActorType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};




