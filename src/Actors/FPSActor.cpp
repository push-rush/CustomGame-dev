#include "../../include/Game.h"
#include "../../include/Actors/FPSActor.h"
#include "../../include/Components/MoveComponent.h"
#include "../../include/Components/FPSCamera.h"
#include "../../include/Components/AudioComponent.h"
#include "../../include/Components/MeshComponent.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Actors/PlaneActor.h"
#include "../../include/Components/BoxComponent.h"
#include "../../include/Actors/BallActor.h"
#include "../../include/Components/BallMove.h"

FPSActor::FPSActor(class Game* game)
: Actor(game)
{
    this->mFootstepCooldown = 0.0f;

    this->mMoveComponent = new MoveComponent(this);
    
    this->mCameraComponent = new FPSCamera(this);
    
    this->mAudioComponent = new AudioComponent(this);
    this->mFootstep = this->mAudioComponent->playEvent("event:/Footstep");
    this->mFootstep.setPaused(true);

    this->mFPSModel = new Actor(game);
    // this->mFPSModel->setPosition(Vector3(10.0f, 10.0f, -10.0f));
    this->mFPSModel->setScale(0.75f);
    
    this->mMeshComponent = new MeshComponent(this->mFPSModel);
    this->mMeshComponent->setMesh(game->getRenderer()->getMesh("../Assets/Meshes/Rifle.gpmesh"));

    this->mBoxComponent = new BoxComponent(this);
    AABB myBox(Vector3(-25.0f, -25.0f, -87.5f),
        Vector3(25.0f, 25.0f, 87.5f)  
    );
    this->mBoxComponent->setObjectBox(myBox);
    this->mBoxComponent->setShouldRotate(false);
}

FPSActor::~FPSActor()
{

}

void FPSActor::actorInput(const uint8_t* keyboard_state)
{
    float forwardSpeed = 0.0;
    float strafeSpeed = 0.0;
    
    // 更新前向速度
    if (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDLK_w])
    {
        forwardSpeed += 400.0f;
    }
    if (keyboard_state[SDL_SCANCODE_S] || keyboard_state[SDLK_s])
    {
        forwardSpeed -= 300.0f;
    }

    // 更新侧向速度
    if (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDLK_a])
    {
        strafeSpeed -= 320.0f;
    }
    if (keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDLK_d])
    {
        strafeSpeed += 320.0f;
    }
    this->mMoveComponent->setForwardSpeed(forwardSpeed);
    this->mMoveComponent->setStrafeSpeed(strafeSpeed);

    // 更新旋转角速度
    // TODO: 启用相对鼠标模式
    int x = 0, y = 0;
    Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
    if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        // 设定鼠标左右的最大移动速度
        const int maxMouseSpeed = 200;
        // 设定最大偏航角速度
        const float maxAngularSpeed = Math::Pi * 24.0;

        float angularSpeed = 0.0;
        if (x != 0)
        {
            // 将鼠标x轴相对坐标转化到[-1.0, 1.0]
            angularSpeed = (float)(x) / maxMouseSpeed;
            angularSpeed *= maxAngularSpeed;
        }
        this->mMoveComponent->setAngularSpeed(angularSpeed);

        // 设定最大俯仰角速度
        const float maxPitchSpeed = Math::Pi * 20.0;
        float pitchSpeed = 0.0;
        if (y != 0)
        {
            // 将鼠标y轴相对坐标转到[-1.0, 1.0]
            pitchSpeed = (float)(y) / maxMouseSpeed;
            pitchSpeed *= maxPitchSpeed;
        }
        this->mMoveComponent->setPitchSpeed(pitchSpeed);
        this->mCameraComponent->setPitchSpeed(pitchSpeed);
        // SDL_Log("angular: %f pitch_speed: %f", angularSpeed, pitchSpeed);
    }
}

void FPSActor::updateActor(float dt)
{
    Actor::updateActor(dt);

    this->mFootstepCooldown -= dt;
	if (!Math::NearZero(this->mMoveComponent->getForwardSpeed()) && this->mFootstepCooldown <= 0.0f)
	{
		this->mFootstep.setPaused(false);
		this->mFootstep.restart();
		this->mFootstepCooldown = 0.5f;
	}

    // 根据角色位置更新FPS模型位置
    const Vector3 modelOffset = Vector3(10.0, 10.0, -10.0);
    Vector3 modelPos = this->getPosition();

    // SDL_Log("Actor pos: [%f %f %f]\n", modelPos.x, modelPos.y, modelPos.z);

    modelPos += this->getForward() * modelOffset.x;
    modelPos += this->getRight() * modelOffset.y;
    modelPos.z += modelOffset.z;

    // SDL_Log("Model pos: [%f %f %f]\n", modelPos.x, modelPos.y, modelPos.z);

    this->mFPSModel->setPosition(modelPos);

    Quaternion quat = this->getRotation();
    quat = Quaternion::Concatenate(quat, Quaternion(this->getRight(), this->mCameraComponent->getPitch()));

    // SDL_Log("Pitch: %f", this->mCameraComponent->getPitch());
    this->mFPSModel->setRotation(quat);
    this->mFPSModel->recomputeWorldTransform(true);
}

void FPSActor::setFootstepSurface(float value)
{
    this->mFootstep.setPaused(true);
    this->mFootstep.setParameter("Surface", value);
}

void FPSActor::fixCollisions()
{
    this->computeWorldTransform();

    const AABB& playerBox = this->mBoxComponent->getWorldBox();
    Vector3 pos = this->getPosition();

    auto& planes = this->getGame()->getPlanes();
    for (auto pa : planes)
    {
        const AABB& planeBox = pa->getBoxComponent()->getWorldBox();
        if (intersect(playerBox, planeBox))
        {
            float dx1 = planeBox.mMin.x - playerBox.mMax.x;
            float dx2 = planeBox.mMax.x - playerBox.mMin.x;
            float dy1 = planeBox.mMin.y - playerBox.mMax.y;
            float dy2 = planeBox.mMax.y - playerBox.mMin.y;
            float dz1 = planeBox.mMin.z - playerBox.mMax.z;
            float dz2 = planeBox.mMax.z - playerBox.mMin.z;

            float dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
            float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
            float dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

            if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
            {
                pos.x += dx;
            }
            else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
            {
                pos.y += dy;
            }
            else
            {
                pos.z += dz;
            }
            this->setPosition(pos);
            this->computeWorldTransform();
        }
    }
}

void FPSActor::shoot()
{
    // 对当前屏幕的中心计算反投影三维点
    Vector3 screenPoint = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 start = this->getGame()->getRenderer()->unProject(screenPoint);
    screenPoint.z = 0.9f;
    Vector3 end = this->getGame()->getRenderer()->unProject(screenPoint);
    Vector3 dir = end - start;
    dir.Normalize();

    // 发射弹球实体
    BallActor* ball = new BallActor(this->getGame());
    ball->getBallMoveComponent()->setPlayer(this);
    ball->setPosition(start + dir * 20.0f);
    ball->rotateToNewForward(dir);

    // 播放射击音效
    this->mAudioComponent->playEvent("event:/Shot");

    // SDL_Log("dir: [%f %f %f]", dir.x, dir.y, dir.z);
}

Actor::EActorType FPSActor::getType() const
{
    return EFPSActor;
}

void FPSActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void FPSActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}