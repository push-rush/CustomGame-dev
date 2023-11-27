#include "../../include/Actors/PlaneActor.h"
#include "../../include/Game.h"
#include "../../include/Components/MeshComponent.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Components/BoxComponent.h"
#include "../../include/Renderers/Mesh.h"

PlaneActor::PlaneActor(class Game* game)
: Actor(game)
{
    this->setScale(10.0f);
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = this->getGame()->getRenderer()->getMesh("../Assets/Meshes/Plane.gpmesh");
    mc->setMesh(mesh);

    this->mBoxComponent = new BoxComponent(this);
    this->mBoxComponent->setObjectBox((*mesh->getBox()));

    // game->addPlane(this);
}

PlaneActor::~PlaneActor()
{
    // this->getGame()->removePlane(this);
}

BoxComponent* PlaneActor::getBoxComponent()
{
    return this->mBoxComponent;
}

Actor::EActorType PlaneActor::getType() const
{
    return EPlaneActor;
}

void PlaneActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);

}

void PlaneActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
    
}