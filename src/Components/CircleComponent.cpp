#include "../../include/Components/CircleComponent.h"
#include "../../include/Actors/Actor.h"
#include "../../include/General/LevelLoader.h"

CircleComponent::CircleComponent()
{

}

CircleComponent::CircleComponent(class Actor* owner)
: Component(owner)
{
    this->mRadius = 0.0f;
}

CircleComponent::~CircleComponent()
{

}

float CircleComponent::getRadius() const
{
    return this->mRadius * this->getActor()->getScale();
}

void CircleComponent::setRadius(float radius)
{
    this->mRadius = radius;
}

// Vector2 CircleComponent::getCenter()
// {
//     return this->getActor()->getPosition();
// }

Vector3 CircleComponent::getCenter()
{
    return this->getActor()->getPosition();
}

bool intersect(CircleComponent* cc1, CircleComponent* cc2)
{
    Vector3 diff = cc1->getCenter() - cc2->getCenter();
    float distSq = diff.LengthSq();

    float radiSum = cc1->getRadius() + cc2->getRadius();
    float radiSq = pow(radiSum, 2);
    
    return distSq <= radiSq;
}

Component::EComponentType CircleComponent::getType() const
{ 
    return ECircleComponent;
}

void CircleComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void CircleComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}