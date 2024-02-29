#pragma once

#include "./Actors/Actor.h"
#include "./Audios/SoundEvent.h"

class CameraActor : public Actor
{
private:
    class MoveComponent* mMoveComponent;
    SoundEvent mFootstep;
    class AudioComponent* mAudioComp;
    float mFootstepCooldown;

public:
    CameraActor(class Game* game);
    ~CameraActor();

    void updateActor(float dt) override;
    void setFootstepSurface(float);
    
    void actorInput(const uint8_t* keyboard_states) override;
    void actorHandleMouse(const int& mouse_wheel) override;
    
    Actor::EActorType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};