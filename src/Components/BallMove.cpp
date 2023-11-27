#include "../../include/Components/BallMove.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/Collision.h"
#include "../../include/Renderers/PhysWorld.h"
#include "../../include/Game.h"
#include "../../include/Actors/TargetActor.h"
#include "../../include/Actors/BallActor.h"
#include "../../include/General/LevelLoader.h"


BallMove::BallMove(class Actor* owner)
: MoveComponent(owner)
{
    
}

BallMove::~BallMove()
{

}

void BallMove::update(float dt)
{
    const float segmentLength = 30.0f;
    Vector3 start = this->getActor()->getPosition();
    Vector3 dir = this->getActor()->getForward();
    Vector3 end = start + dir * segmentLength;
    LineSegment ls(start, end);

    // 测试线段在物理空间中的碰撞
    PhysWorld* phys = this->getActor()->getGame()->getPhysWorld();
    CollisionInfo info;
    if (phys->segmentCast(ls, info) && info.mCollActor != this->mPlayer)
    {
        // 如果碰撞，则根据法线对当前弹球的方向进行反射
        dir = Vector3::Reflect(dir, info.mCollPlaneNormal);
        this->getActor()->rotateToNewForward(dir);

        TargetActor* target = static_cast<TargetActor*>(info.mCollActor);
        if (target)
        {
            static_cast<BallActor*>(this->getActor())->hitTarget();
        }
    }

    // 通过基类对前行速度进行更新
    MoveComponent::update(dt);
}

void BallMove::setPlayer(class Actor* player)
{
    this->mPlayer = player;
}

Component::EComponentType BallMove::getType() const
{
    return EBallMove;
}

void BallMove::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);
}

void BallMove::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
}