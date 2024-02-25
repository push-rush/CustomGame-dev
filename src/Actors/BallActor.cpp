#include "../../include/Actors/BallActor.h"
#include "../../include/Game.h"
#include "../../include/Components/MeshComponent.h"
#include "../../include/Components/BallMove.h"
#include "../../include/Components/AudioComponent.h"
#include "../../include/Renderers/Renderer.h"

BallActor::BallActor(class Game* game)
: Actor(game)
{
    // 设置弹球的生命周期
    this->mLifeSpan = 2.0f;

    MeshComponent* mc = new MeshComponent(this);
    mc->setMesh(game->getRenderer()->getMesh("../Assets/Meshes/Sphere.gpmesh"));

    this->mBallMoveComponent = new BallMove(this);
    this->mBallMoveComponent->setForwardSpeed(1500.0f);

    this->mAudioComponent = new AudioComponent(this);
}

BallActor::~BallActor()
{

}

void BallActor::updateActor(float dt)
{
    Actor::updateActor(dt);

    this->mLifeSpan -= dt;
    if (mLifeSpan <= 0.0f)
    {
        this->setState(EDead);
    }

    // SDL_Log("forward: %f %f %f", this->getForward().x, this->getForward().y, this->getForward().z);
}

void BallActor::hitTarget()
{   // 击中目标播放音效
    this->mAudioComponent->playEvent("event:/Ding");
}

BallMove* BallActor::getBallMoveComponent()
{
    return this->mBallMoveComponent;
}

Actor::EActorType BallActor::getType() const
{
    return EBallActor;
}

void BallActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void BallActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}