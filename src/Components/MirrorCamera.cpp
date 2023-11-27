#include "../../include/Components/MirrorCamera.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/General/LevelLoader.h"

MirrorCamera::MirrorCamera(Actor* owner)
: CameraComponent(owner)
{
    this->mHorizonDist = 150.0f;
	this->mVerticalDist = 200.0f;
	this->mTargetDist = 400.0f;
}

MirrorCamera::~MirrorCamera()
{

}

void MirrorCamera::update(float dt)
{
    // SDL_Log("[MirrorCamera] Updating...");

    CameraComponent::update(dt);

    // Vector3 idealPos = this->computeCameraPos();
    // Vector3 target = this->getActor()->getPosition() - this->getActor()->getForward() * this->mTargetDist;

    // Matrix4 view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);
    snapToIdeal();

    // this->getActor()->getGame()->getRenderer()->setMirrorView(view);
}

void MirrorCamera::snapToIdeal()
{
    Vector3 IdealPos = this->computeCameraPos();
    Vector3 target = this->getActor()->getPosition() - this->getActor()->getForward() * this->mTargetDist;

    Matrix4 view = Matrix4::CreateLookAt(IdealPos, target, Vector3::UnitZ);
    this->getActor()->getGame()->getRenderer()->setMirrorView(view);
}

void MirrorCamera::setHorzDist(float dist)
{
    this->mHorizonDist = dist;
}

void MirrorCamera::setVertDist(float dist)
{
    this->mVerticalDist = dist;
}

void MirrorCamera::setTargetDist(float dist)
{
    this->mTargetDist = dist;
}

Vector3 MirrorCamera::computeCameraPos() const
{
    Vector3 cameraPos = this->getActor()->getPosition();
    cameraPos += this->getActor()->getForward() * this->mHorizonDist;
    cameraPos += this->getActor()->getUp() * this->mVerticalDist;
    return cameraPos;
}

Component::EComponentType MirrorCamera::getType() const
{
    return EMirrorCamera;
}

void MirrorCamera::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void MirrorCamera::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}