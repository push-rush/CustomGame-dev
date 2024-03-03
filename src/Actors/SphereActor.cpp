#include "../../include/Actors/SphereActor.h"

#include "../../include/Components/CircleComponent.h"
#include "../../include/Components/MeshComponent.h"

#include "../../include/Renderers/Mesh.h"
#include "../../include/Renderers/Collision.h"
#include "../../include/Renderers/Renderer.h"

#include "../../include/Game.h"

SphereActor::SphereActor(Game* game) : Actor(game)
{
    this->setScale(3.0f);
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = game->getRenderer()->getMesh("../Assets/Meshes/Sphere.gpmesh");
    mc->setMesh(mesh);

    this->mCircleComp = new CircleComponent(this);

    auto box = mesh->getBox(); 
    float r = (box->mMax.x - box->mMin.x) * 0.5f; 
    r += (box->mMax.y - box->mMin.y) * 0.5f;
    r *= 0.5f; 
    r += (box->mMax.z - box->mMin.z) * 0.5f; 
    r *= 0.5f;
    this->mCircleComp->setRadius(r);
}

SphereActor::~SphereActor()
{

}

Actor::EActorType SphereActor::getType() const
{
    return Actor::ESphereActor;
}

void SphereActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);

}

void SphereActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
    
}