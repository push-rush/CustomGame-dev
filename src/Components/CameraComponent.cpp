#include "../../include/Game.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Audios/AudioSystem.h"
#include "../../include/Components/CameraComponent.h"
#include "../../include/General/LevelLoader.h"


CameraComponent::CameraComponent(class Actor* owner)
: Component(owner)
{

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::setViewMatrix(const Matrix4& view)
{
    class Game* game = this->getActor()->getGame();
    game->getRenderer()->setViewMatrix(view);
    game->getAudioSystem()->setListener(view);
}

Component::EComponentType CameraComponent::getType() const
{
    return ECameraComponent;
}

void CameraComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void CameraComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}