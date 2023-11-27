#include "../../include/Components/TargetComponent.h"
#include "../../include/Game.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/HUD.h"
#include "../../include/General/LevelLoader.h"

TargetComponent::TargetComponent(class Actor* owner)
: Component(owner)
{
    SDL_Log("[TargetComponent] Add target comp...");
    owner->getGame()->getHUD()->addTargetComp(this);
}

TargetComponent::~TargetComponent()
{
    this->getActor()->getGame()->getHUD()->removeTargetComp(this);
}

Component::EComponentType TargetComponent::getType() const
{
    return ETargetComponent;
}

void TargetComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void TargetComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
}