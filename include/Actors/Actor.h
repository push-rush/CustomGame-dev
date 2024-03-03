#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>

#include <rapidjson.h>
#include <document.h>

#include "../General/Cmath.h"
#include "../General/Random.h"

using namespace std;

class Actor
{
public:
    enum EActorType
    {
        EActor,
        EAsteroid,
        EBallActor,
        ECameraActor,
        EFollowActor,
        EFPSActor,
        ELaser,
        EOrbitActor,
        EPlaneActor,
        EShip,
        ESplineActor,
        ETargetActor,
        ESphereActor,

        NUM_ACTOR_TYPES
    };

    static const char* typeNames[NUM_ACTOR_TYPES];

public:
    Actor(/* args */);
    Actor(class Game* game);
    virtual ~Actor();

    enum State
    {
        EActive,
        EPaused,
        EDead,
        ESelected,
        EClicked
    };

    // 组件/角色更新
    void update(float dt);
    void updateComponents(float dt);
    virtual void updateActor(float dt);

    // 添加/删除组件
    void addComponent(class Component* component);
    void removeComponent(class Component* component);

    // Getters
    float getScale() const;
    State getState() const;
    class Vector3 getPosition();
    Matrix4 getWorldTransform();
    class Game* getGame() const;
    class Vector3 getUp() const;
    class Vector3 getRight() const;
    class Vector3 getForward() const;
    class Quaternion getRotation() const;
    virtual class SpriteComponent* getSpriteComponent() const;
    std::vector<class Component*>& getComponents();
    virtual EActorType getType() const;
    bool reUpdateColliBox() const;

    // Setters
    void setState(State);
    void setScale(float);
    void setRotation(Quaternion);
    void setFixedPosition(class Vector3 pos);
    virtual void setPosition(class Vector3 pos);
    void setWorldTranform(const Matrix4& world);
    void updateColliBox(const bool& update);

    // 外部输入处理
    void processInput(const uint8_t* keyboard_state);
    void handleMouseWheel(const int& mouse_wheel);
    virtual void actorInput(const uint8_t* keyboard_state);
    virtual void actorHandleMouse(const int& mouse_wheel);
    
    // 计算世界变换矩阵
    void computeWorldTransform();
    void recomputeWorldTransform(bool);
    void actorComputerWorldTransform();

    // 碰撞反弹方向计算
    void rotateToNewForward(const Vector3& forward);

    // 加载属性
    virtual void loadProperties(const rapidjson::Value& inObj);

    // 保存属性
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props);

    template <typename T>
    static Actor* create(class Game* game, const rapidjson::Value& inObj)
    {
        T* t = new T(game);
        // 加载指定属性
        t->loadProperties(inObj);

        return t;
    }

private:
    std::vector<class Component*> mComponents; 
    class Game* mGame;

    State mState;
    float mScale;               // 对象的缩放百分比
    class Vector3 mPosition;    // 角色位置
    Quaternion mQuat;           // 角色朝向
    // class Vector2 mPosition; // 对象的中心位置
    // float mRotation;   // 旋转角度（rad）
    
    Matrix4 mWorldTransform;    // 角色时间变换矩阵
    bool mRecomputeWorldTransform;

    bool mUpdateColliBox;
};