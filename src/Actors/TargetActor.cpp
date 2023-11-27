#include "../../include/Actors/TargetActor.h"
#include "../../include/Game.h"
#include "../../include/Components/MeshComponent.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Components/BoxComponent.h"
#include "../../include/Renderers/Mesh.h"
#include "../../include/Components/TargetComponent.h"

TargetActor::TargetActor(class Game* game)
: Actor(game)
{
    this->setRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    this->mMeshComponent = new MeshComponent(this);
    Mesh* mesh = game->getRenderer()->getMesh("../Assets/Meshes/Target.gpmesh");
    this->mMeshComponent->setMesh(mesh);

    // 添加碰撞组件
    this->mBoxComponent = new BoxComponent(this);
    this->mBoxComponent->setObjectBox(*(mesh->getBox()));

    this->mTargetComponent = new TargetComponent(this);
}

TargetActor::~TargetActor()
{
    
}

Actor::EActorType TargetActor::getType() const
{
    return ETargetActor;
}

void TargetActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void TargetActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}