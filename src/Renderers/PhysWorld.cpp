#include "../../include/Renderers/PhysWorld.h"
#include "../../include/Renderers/Collision.h"

#include "../../include/Components/BoxComponent.h"
#include "../../include/Components/CircleComponent.h"

#include "../../include/Actors/Actor.h"


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

    // auto st = segLine.mStart;
    // sort(this->mBoxComponents.begin(), this->mBoxComponents.end(), [st](BoxComponent*& b1, BoxComponent*& b2){
    //     return (b1->getActor()->getPosition() - st).LengthSq() < (b2->getActor()->getPosition() - st).LengthSq();
    // });

    Vector3 norm;
    for (auto box : this->mBoxComponents)
    {
        float t = 0.0f;
        if (intersect(segLine, box->getWorldBox(), t, norm))
        {
            if (t < closestT)
            {
                outCollInfo.mCastPoint = segLine.PointOnSegment(t);
                outCollInfo.mCollPlaneNormal = norm;
                outCollInfo.mCollBoxComp = box;
                outCollInfo.mCollCircleComp = nullptr;
                outCollInfo.mCollActor = box->getActor();
                collided = true;

                closestT = t;

                // break;
            }

            // auto dist = box->getWorldBox().mMax - box->getWorldBox().mMin;
            // SDL_Log("[PhysWorld] Box size: w: %.2f h: %.2f l: %.2f", dist.x, dist.y, dist.z);
        }
    }

    for (auto circle : this->mCircleComponents)
    {
        float t = 0.0f;

        Sphere s = {circle->getCenter(), circle->getRadius()};
        if (intersect(segLine, s, t))
        {
            if (t < closestT)
            {
                outCollInfo.mCastPoint = segLine.PointOnSegment(t);
                outCollInfo.mCollPlaneNormal = norm;
                outCollInfo.mCollBoxComp = nullptr;
                outCollInfo.mCollCircleComp = circle;
                outCollInfo.mCollActor = circle->getActor();
                collided = true;

                closestT = t;
                break;
            }
        }
    }

    return collided;
}

/**
 * TODO: 函数实现存在问题，待改
*/
bool PhysWorld::RayCast(const class Ray& ray, CollisionInfo& outCollInfo)
{
    bool collided = false;
    float closestT = Math::Infinity;

    auto st = ray.origin;
    sort(this->mBoxComponents.begin(), this->mBoxComponents.end(), [st](BoxComponent*& b1, BoxComponent*& b2){
        return (b1->getActor()->getPosition() - st).LengthSq() < (b2->getActor()->getPosition() - st).LengthSq();
    });

    // auto st = ray.origin;
    Vector3 colli_point = Vector3{0.0f, 0.0f, 0.0f};
    for (auto box : this->mBoxComponents)
    {
        float t = 0.0f;
        if (intersect(ray, box->getWorldBox(), colli_point))
        {
            t = (colli_point - st).LengthSq();
            if (t < closestT)
            {
                outCollInfo.mCastPoint = colli_point;
                outCollInfo.mCollPlaneNormal = Vector3{0.0f, 0.0f, 0.0f};
                outCollInfo.mCollBoxComp = box;
                outCollInfo.mCollCircleComp = nullptr;
                outCollInfo.mCollActor = box->getActor();
                collided = true;

                closestT = t;
                // break;
            }

            SDL_Log("[PhysWorld] actor type: %d dist: %.2f", (int)(box->getActor()->getType()), t);
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

void PhysWorld::addCircleComponent(class CircleComponent* sphere)
{
    this->mCircleComponents.emplace_back(sphere);
}

void PhysWorld::removeCircleComponent(class CircleComponent* sphere)
{
    for (auto iter = this->mCircleComponents.begin(); iter != this->mCircleComponents.end();)
    {
        if ((*iter) == sphere)
        {
            iter = this->mCircleComponents.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }
}

std::vector<BoxComponent*>& PhysWorld::getBoxComponents()
{
    return this->mBoxComponents;
}