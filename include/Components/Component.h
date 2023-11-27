#pragma once

#include <stdio.h>
#include <iostream>
#include <document.h>
#include <rapidjson.h>

#include "./General/Cmath.h"

#define VNAME(value) (#value)

class Component
{
public:
    enum EComponentType
    {
        EComponent,
        EAudioComponent,
        EBallMove,
        EBoxComponent,
        ECameraComponent,
        ECircleComponent,
        EFollowCamera,
        EFPSCamera,
        EInputComponent,
        EMeshComponent,
        EMirrorCamera,
        EMoveComponent,
        EOrbitCamera,
        EPointLightComponent,
        ESkeletalMeshComponent,
        ESplineCamera,
        ESpriteComponent,
        ETargetComponent,

        NUM_COMPONENT_TYPES
    };

    static const char* TypeNames[NUM_COMPONENT_TYPES];

private:
    class Actor* mOwner = nullptr;
    int mUpdateOrder;

public:
    Component(/* args */);
    Component(class Actor* owner, int updateOrder = 100);
    virtual ~Component();

    virtual void update(float dt);
    virtual void onUpdateWorldTransform();
    virtual void processInput(const uint8_t* keyboard_state);

    int getUpdateOrder() const;
    class Actor* getActor() const; 

    template <typename T>
    static Component* create(class Actor* actor, const rapidjson::Value& inObj)
    {
        T* t = new T(actor);
        t->loadProperties(inObj);

        return t;
    }

    virtual void loadProperties(const rapidjson::Value& inObj);
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props);

    virtual EComponentType getType() const;
};