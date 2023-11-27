#include "../../include/Actors/OrbitActor.h"
#include "../../include/Components/OrbitCamera.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Components/MeshComponent.h"
#include "../../include/Components/MoveComponent.h"

OrbitActor::OrbitActor(class Game* game)
: Actor(game)
{
    this->mCameraComponent = new OrbitCamera(this);
    this->mMeshComponent = new MeshComponent(this);
    this->mMeshComponent->setMesh(game->getRenderer()->getMesh("../../Assets/RacingCar.gpmesh"));

    this->setPosition(Vector3(0.0f, 0.0f, -100.0f));
    this->mMoveComponent= new MoveComponent(this);
}

OrbitActor::~OrbitActor()
{

}

void OrbitActor::actorInput(const uint8_t* keyboard_state)
{
    float forwardSpeed = 0.0;
    // float strafeSpeed = 0.0;
    float angularSpeed = 0.0;
    
    // 更新前向速度
    if (keyboard_state[SDL_SCANCODE_W])
    {
        forwardSpeed += 550.0f;
    }
    if (keyboard_state[SDL_SCANCODE_S])
    {
        forwardSpeed -= 450.0f;
    }

    // 更新侧向速度
    if (keyboard_state[SDL_SCANCODE_A])
    {
        // strafeSpeed -= 320.0f;
        angularSpeed -= Math::Pi;
    }
    if (keyboard_state[SDL_SCANCODE_D])
    {
        // strafeSpeed += 320.0f;
        angularSpeed += Math::Pi;
    }
    this->mMoveComponent->setForwardSpeed(forwardSpeed);
    // this->mMoveComponent->setStrafeSpeed(strafeSpeed);
    this->mMoveComponent->setAngularSpeed(angularSpeed);

    // 更新旋转角速度
    // TODO: 启用相对鼠标模式
    int x = 0, y = 0;
    angularSpeed = 0.0f;
    Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
    if (buttons && SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        // 设定鼠标左右的最大移动速度
        const int maxMouseSpeed = 200;
        // 设定最大偏航角速度
        const float maxAngularSpeed = Math::Pi * 24.0;

        if (x != 0)
        {
            // 将鼠标x轴相对坐标转化到[-1.0, 1.0]
            angularSpeed = (float)(x) / maxMouseSpeed;
            angularSpeed *= maxAngularSpeed;
        }
        this->mCameraComponent->setYawSpeed(angularSpeed);

        // 设定最大俯仰角速度
        const float maxPitchSpeed = Math::Pi * 20.0;
        float pitchSpeed = 0.0;
        if (y != 0)
        {
            // 将鼠标y轴相对坐标转到[-1.0, 1.0]
            pitchSpeed = (float)(y) / maxMouseSpeed;
            pitchSpeed *= maxPitchSpeed;
        }
        // this->mCameraComponent->setPitchSpeed(pitchSpeed);
        this->mCameraComponent->setPitchSpeed(pitchSpeed);
        this->recomputeWorldTransform(true);
    }
}

Actor::EActorType OrbitActor::getType() const
{
    return EOrbitActor;
}

void OrbitActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void OrbitActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}