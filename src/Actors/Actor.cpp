#include "../../include/Actors/Actor.h"
#include "../../include/Components/Component.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Components/SpriteComponent.h"
#include "../../include/General/LevelLoader.h"
#include "../../include/Actors/TargetActor.h"

const char* Actor::typeNames[Actor::NUM_ACTOR_TYPES] = 
{
    "Actor",
    "Asteroid",
    "BallActor",
    "CameraActor",
    "FollowActor",
    "FPSActor",
    "Laser",
    "OrbitActor",
    "PlaneActor",
    "Ship",
    "SplineActor",
    "TargetActor"
};

Actor::Actor()
{
    this->mScale = 1.0f;
    this->mState = EActive;
    this->mPosition = Vector3::Zero;
    this->mQuat = Quaternion::Identity;
    this->mRecomputeWorldTransform = true;
    this->mWorldTransform = Matrix4::Identity;
}

Actor::Actor(class Game* game)
{
    this->mGame = game;
    this->mScale = 1.0f;
    this->mState = EActive;
    this->mPosition = Vector3::Zero;
    this->mQuat = Quaternion::Identity;
    this->mRecomputeWorldTransform = true;
    this->mWorldTransform = Matrix4::Identity;

    game->addActor(this);
}

Actor::~Actor()
{   
    // 释放角色和组件资源
    while (!mComponents.empty())
    {
        if (mComponents.back() != nullptr)
        {
            Component* comp = mComponents.back();
            mComponents.pop_back();
            delete comp;
            comp = nullptr;
        }
    }
    this->getGame()->removeActor(this);
}

Actor::State Actor::getState() const
{
    return mState;
}

void Actor::setState(State state)
{
    mState = state;
}

void Actor::update(float dt)
{
    if (this->getState() == EActive)
    {
        this->computeWorldTransform();

        this->updateComponents(dt);
        this->updateActor(dt);

        this->computeWorldTransform();
    }
}

void Actor::updateComponents(float dt)
{
    for (auto iter = this->mComponents.begin(); iter != this->mComponents.end();)
    {
        if ((*iter) != nullptr)
        {
            (*iter)->update(dt);
            ++iter;
        }
        else
        {
            iter = this->mComponents.erase(iter);
        }
    }

    // for (auto coms : mComponents)
    // {
    //     if (coms != nullptr)
    //     {
    //         coms->update(dt);
    //     }
    // }
}
    
void Actor::updateActor(float dt)
{

}

void Actor::addComponent(class Component* component)
{
    int addOrder = component->getUpdateOrder();
    auto iter = this->mComponents.begin(); 
    for (; iter != this->mComponents.end(); ++iter)
    {
        if (addOrder < (*iter)->getUpdateOrder())
        {
            break;
        }
    }

    this->mComponents.insert(iter, component);
}

void Actor::removeComponent(class Component* component)
{
    for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
    {
        if ((*iter) == component)
        {
            iter = mComponents.erase(iter);
            break;
        }
    }
}

// float Actor::getRotation() const
// {
//     return mRotation;
// }

// void Actor::setRotation(float rot)
// {
//     this->mRotation = rot;
// }

// Vector2 Actor::getPosition()
// {
//     return mPosition;
// }

// void Actor::setPosition(Vector2 pos)
// {
//     this->mPosition = pos;
// }

// void Actor::setFixedPosition(class Vector2 pos)
// {
//     this->mPosition = pos;
// }

// Vector2 Actor::getForward() const
// {
//     return Vector2(Math::Cos(this->mRotation), Math::Sin(this->mRotation));
// }

Quaternion Actor::getRotation() const
{
    return this->mQuat;
}

void Actor::setRotation(Quaternion quat)
{
    this->mQuat = quat; 
}

Vector3 Actor::getForward() const
{
    return Vector3::Transform(Vector3::UnitX, this->mQuat);
}

Vector3 Actor::getPosition()
{
    return this->mPosition;
}

void Actor::setPosition(Vector3 pos)
{
    this->mPosition = pos;
}   

void Actor::setFixedPosition(Vector3 pos)
{
    this->mPosition = pos;
}

float Actor::getScale() const
{
    return mScale;
}

void Actor::setScale(float scale)
{
    this->mScale = scale;    
}

void Actor::processInput(const uint8_t* keyboard_state)
{
    if (mState == EActive)
    {
        for (auto comp : mComponents)
        {
            comp->processInput(keyboard_state);
        }
        actorInput(keyboard_state);
    }
}

void Actor::actorInput(const uint8_t* keyboard_state)
{

}

class Game* Actor::getGame() const
{
    return this->mGame;
}


void Actor::computeWorldTransform()
{
    if (mRecomputeWorldTransform)
    {
        this->mWorldTransform = Matrix4::CreateScale(this->mScale);
        // this->mWorldTransform *= Matrix4::CreateRotationZ(this->mRotation);
        this->mWorldTransform *= Matrix4::CreateFromQuaternion(this->mQuat);
        this->mWorldTransform *= Matrix4::CreateTranslation(this->mPosition);

        for (auto comp : this->mComponents)
        {
            comp->onUpdateWorldTransform();
        }

        mRecomputeWorldTransform = false;
    }
}

Matrix4 Actor::getWorldTransform()
{
    return this->mWorldTransform;
}

void Actor::recomputeWorldTransform(bool is_recomputed)
{
    this->mRecomputeWorldTransform = is_recomputed;
}

SpriteComponent* Actor::getSpriteComponent() const
{
    return nullptr;
}

Vector3 Actor::getRight() const
{
    return Vector3::Transform(Vector3::UnitY, this->mQuat);
}

Vector3 Actor::getUp() const
{
    return Vector3::Transform(Vector3::UnitZ, this->mQuat);
}

/**
 * 将角色旋转到反弹后的方向
*/
void Actor::rotateToNewForward(const Vector3& forward)
{
    float dot = Vector3::Dot(Vector3::UnitX, forward);
    float angle = Math::Acos(dot);

    if (dot > 0.999f)
    {
        this->setRotation(Quaternion::Identity);
    }
    else if (dot < -0.999f)
    {
        this->setRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    }
    else
    {
        Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
        axis.Normalize();
        this->setRotation(Quaternion(axis, angle));
        
        // SDL_Log("AIXS: [%f %f %f] angle: %f", axis.x, axis.y, axis.z, angle);
    }
}

void Actor::loadProperties(const rapidjson::Value& inObj)
{
    std::string state;
    if (JsonHelper::getString(inObj, "state", state))
    {
        if (state == "active")
        {
            this->setState(EActive);
        }
        else if (state == "paused")
        {
            this->setState(EPaused);
        }
        else if (state == "dead")
        {
            this->setState(EDead);
        }
    }

    // 加载角色位置、旋转和缩放，并且计算世界变换矩阵
    JsonHelper::getVector3(inObj, "position", this->mPosition);
    JsonHelper::getRotation(inObj, "rotation", this->mQuat);
    JsonHelper::getScale(inObj, "scale", this->mScale);
    this->computeWorldTransform();
}

std::vector<class Component*>& Actor::getComponents()
{
    return this->mComponents;
}

void Actor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    std::string state = "active";
    if (this->mState == EPaused)
    {
        state = "paused";
    }
    else if (this->mState == EDead)
    {
        state = "dead";
    }

    JsonHelper::addString(alloc, props, "state", state);
    JsonHelper::addScale(alloc, props, "scale", this->mScale);
    JsonHelper::addVector3(alloc, props, "position", this->mPosition);
    JsonHelper::addRotation(alloc, props, "rotation", this->mQuat);
}

Actor::EActorType Actor::getType() const
{
    return EActor;
}