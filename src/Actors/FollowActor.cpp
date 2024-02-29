#include "../../include/Actors/FollowActor.h"
// #include "../../include/MeshComponent.h"
#include "../../include/Components/SkeletalMeshComponent.h"
#include "../../include/Components/MoveComponent.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Components/FollowCamera.h"
#include "../../include/Components/AudioComponent.h"
#include "../../include/Audios/AudioSystem.h"
#include "../../include/Components/MirrorCamera.h"

FollowActor::FollowActor(class Game* game)
: Actor(game)
{
    this->mMoving = false;
    this->mFootstepCooldown = 0.0f;

    // this->mMeshComponent = new MeshComponent(this);
    this->mSkeletalComponent = new SkeletalMeshComponent(this);
    // this->mMeshComponent->setMesh(game->getRenderer()->getMesh("../Assets/Meshes/RacingCar.gpmesh"));
    this->mSkeletalComponent->setMesh(game->getRenderer()->getMesh("../Assets/Meshes/CatWarrior.gpmesh"));
    this->mSkeletalComponent->setSkeleton(game->getSkeleton("../Assets/Skeletons/CatWarrior.gpskel"));
    float dura = this->mSkeletalComponent->playAnimation(game->getAnimation("../Assets/Animations/CatActionIdle.gpanim"));
    SDL_Log("Animation duration: %f", dura);

    this->setPosition(Vector3(0.0f, 0.0f, -100.0f));

    this->mMoveComponent = new MoveComponent(this);
    this->mFollowComponent = new FollowCamera(this);
    this->mFollowComponent->snapToIdeal();

    this->mMirrorCameraComponent = new MirrorCamera(this);
    this->mMirrorCameraComponent->snapToIdeal();

    this->mAudioComponent = new AudioComponent(this);
    this->mFootstep = this->mAudioComponent->playEvent("event:/Footstep");
    this->mFootstep.setPaused(true);
}

FollowActor::~FollowActor()
{

}

void FollowActor::actorInput(const uint8_t* keyboard_state)
{
    float forwardSpeed = 0.0f;
    // float strafeSpeed = 0.0;
    float angularSpeed = 0.0f;

    // 更新前向速度
    if (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDLK_w])
    {
        forwardSpeed += 650.0f;
    }
    if (keyboard_state[SDL_SCANCODE_S] || keyboard_state[SDLK_s])
    {
        forwardSpeed -= 350.0f;
    }

    // 更新转向速度
    if (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDLK_a])
    {
        angularSpeed -= Math::Pi;
    }
    if (keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDLK_d])
    {
        angularSpeed += Math::Pi;
    }

    // SDL_Log("[FollowActor] actor input...");

    if (!Math::NearZero(forwardSpeed) && !this->mMoving)
    {
        this->mMoving = true;
        this->mSkeletalComponent->playAnimation(this->getGame()->getAnimation("../Assets/Animations/CatRunSprint.gpanim"), 1.25f);

        this->mFollowComponent->setHorizonDist(400.0f);
        this->mFollowComponent->setVerticalDist(270.0f);
    }
    else if (this->mMoving && Math::NearZero(forwardSpeed))
    {   
        this->mMoving = false;
        this->mSkeletalComponent->playAnimation(this->getGame()->getAnimation("../Assets/Animations/CatActionIdle.gpanim"));

        this->mFollowComponent->setHorizonDist(360.0f);
        this->mFollowComponent->setVerticalDist(250.0f);
    }

    this->mMoveComponent->setForwardSpeed(forwardSpeed);
    this->mMoveComponent->setAngularSpeed(angularSpeed);
}

void FollowActor::actorHandleMouse(const int& mouse_wheel)
{

}

void FollowActor::setFootstepSurface(float value)
{
    this->mFootstep.setPaused(true);
    this->mFootstep.setParameter("Surface", value);
}

void FollowActor::updateActor(float dt)
{
    Actor::updateActor(dt);

    this->mFootstepCooldown -= dt;
	if (!Math::NearZero(this->mMoveComponent->getForwardSpeed()) && this->mFootstepCooldown <= 0.0f)
	{
		this->mFootstep.setPaused(false);
		this->mFootstep.restart();
		this->mFootstepCooldown = 0.5f;

        SDL_Log("【FollowActor】 Update actor...");
	}
}

// void FollowActor::setFootstepSurface(float)
// {

// }

Actor::EActorType FollowActor::getType() const
{
    return EFollowActor;
}

void FollowActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void FollowActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}