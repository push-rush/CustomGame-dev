#include "../../include/Renderers/PhysWorld.h"
#include "../../include/Components/BoxComponent.h"
#include "../../include/Renderers/Collision.h"

PhysWorld::PhysWorld(class Game* game)
{
    this->mGame = game;
}

PhysWorld::~PhysWorld()
{

}

void PhysWorld::addBoxComponent(class BoxComponent* box)
{
    this->mBoxComponents.emplace_back(box);
}

void PhysWorld::removeBoxComponent(class BoxComponent* box)
{
    for (auto iter = this->mBoxComponents.begin(); iter != this->mBoxComponents.end();)
    {
        if ((*iter) == box)
        {
            iter = this->mBoxComponents.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }
}

bool PhysWorld::segmentCast(const class LineSegment& segLine, CollisionInfo& outCollInfo)
{
    bool collided = false;
    float closestT = Math::Infinity;

    Vector3 norm;
    for (auto box : this->mBoxComponents)
    {
        float t = 0.0;
        if (intersect(segLine, box->getWorldBox(), t, norm))
        {
            if (t < closestT)
            {
                outCollInfo.mCastPoint = segLine.PointOnSegment(t);
                outCollInfo.mCollPlaneNormal = norm;
                outCollInfo.mCollBoxComp = box;
                outCollInfo.mCollActor = box->getActor();
                collided = true;

                closestT = t;
            }
        }
    }
    return collided;
}

void PhysWorld::testPairwise(std::function<void(class Actor*, class Actor*)> f)
{
    for (size_t i = 0; i < this->mBoxComponents.size(); ++i)
    {
        BoxComponent* a = this->mBoxComponents[i];
        for (size_t j = i + 1; j < this->mBoxComponents.size(); ++j)
        {
            BoxComponent* b = this->mBoxComponents[j];
            if (intersect(a->getWorldBox(), b->getWorldBox()))
            {
                // 调用对碰撞组件的处理函数
                f(a->getActor(), b->getActor());
            }
        }
    }
}

void PhysWorld::testSweepAndPrune(std::function<void(class Actor*, class Actor*)> f)
{
    std::sort(this->mBoxComponents.begin(), this->mBoxComponents.end(),
        [](BoxComponent* a, BoxComponent* b)
        {
            return a->getWorldBox().mMin.x < b->getWorldBox().mMin.x;
        }
    );

    for (size_t i = 0; i < this->mBoxComponents.size(); ++i)
    {
        BoxComponent* a = this->mBoxComponents[i];
        float max = a->getWorldBox().mMax.x;
        for (size_t j = i + 1; j < this->mBoxComponents.size(); ++j)
        {
            BoxComponent* b = this->mBoxComponents[j];
            if (b->getWorldBox().mMin.x > max)
            {
                break;
            }
            else if (intersect(a->getWorldBox(), b->getWorldBox()))
            {
                f(a->getActor(), b->getActor());
            }
        }
    }
}