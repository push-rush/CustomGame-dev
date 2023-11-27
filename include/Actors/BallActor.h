#pragma once

#include "./Actor.h"

class BallActor : public Actor
{
private:
    class AudioComponent* mAudioComponent;
    class BallMove* mBallMoveComponent;
    float mLifeSpan;

public:
    BallActor(class Game* game);
    ~BallActor();

    void updateActor(float dt) override;
    void hitTarget();
    
    // getters
    class BallMove* getBallMoveComponent();
    Actor::EActorType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};