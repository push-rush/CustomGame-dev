#include "../../include/Actors/CameraActor.h"
#include "../../include/Game.h"
#include "../../include/Components/MoveComponent.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Components/AudioComponent.h"
#include "../../include/Audios/AudioSystem.h"

CameraActor::CameraActor(class Game* game)
: Actor(game)
{
	this->mFootstepCooldown = 0.0f;
    this->mMoveComponent = new MoveComponent(this);
	this->mAudioComp = new AudioComponent(this);
	this->mFootstep = this->mAudioComp->playEvent("event:/Footstep");
	this->mFootstep.setPaused(true);
}

CameraActor::~CameraActor()
{

}

void CameraActor::updateActor(float dt)
{
    Actor::updateActor(dt);

	this->mFootstepCooldown -= dt;
	if (!Math::NearZero(this->mMoveComponent->getForwardSpeed()) && this->mFootstepCooldown <= 0.0f)
	{
		this->mFootstep.setPaused(false);
		this->mFootstep.restart();
		this->mFootstepCooldown = 0.5f;
	}

    // 计算相机角色
    Vector3 cameraPos = this->getPosition();
    Vector3 target = cameraPos + this->getForward() * 100.0f;
    Vector3 up = Vector3::UnitZ; 

    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    this->getGame()->getRenderer()->setViewMatrix(view);
	this->getGame()->getAudioSystem()->setListener(view);

	SDL_Log("forward: [%f %f %f]", this->getForward().x, this->getForward().y, this->getForward().z);
}

void CameraActor::actorInput(const uint8_t* keyboard_states)
{
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0;

    // W(前) S(后) A(逆时针旋转) D(顺时针旋转)
	if (keyboard_states[SDL_SCANCODE_W])
	{
		forwardSpeed += 350.0f;
	}
	if (keyboard_states[SDL_SCANCODE_S])
	{
		forwardSpeed -= 250.0f;
	}
	if (keyboard_states[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::Pi * 1.5f;
	}
	if (keyboard_states[SDL_SCANCODE_D])
	{
		angularSpeed += Math::Pi * 1.5f;
	}

    this->mMoveComponent->setForwardSpeed(forwardSpeed);
    this->mMoveComponent->setAngularSpeed(angularSpeed);
}

void CameraActor::setFootstepSurface(float value)
{
	this->mFootstep.setPaused(true);
	this->mFootstep.setParameter("Surface", value);
}

Actor::EActorType CameraActor::getType() const
{
    return ECameraActor;
}

void CameraActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void CameraActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}