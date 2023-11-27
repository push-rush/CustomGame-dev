#include "../../include/Components/BoxComponent.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/Collision.h"
#include "../../include/Game.h"
#include "../../include/Renderers/PhysWorld.h"
#include "../../include/General/LevelLoader.h"


BoxComponent::BoxComponent(class Actor* owner)
: Component(owner), mObjectBox(Vector3::Zero, Vector3::Zero),
mWorldBox(Vector3::Zero, Vector3::Zero)
{
    this->mShouldRotate = true;
    owner->getGame()->getPhysWorld()->addBoxComponent(this);
}

BoxComponent::~BoxComponent()
{
    this->getActor()->getGame()->getPhysWorld()->removeBoxComponent(this);
}

void BoxComponent::onUpdateWorldTransform()
{
    this->mWorldBox = this->mObjectBox;
    
    // 缩放
    this->mWorldBox.mMin *= this->getActor()->getScale();
    this->mWorldBox.mMax *= this->getActor()->getScale();

    // 旋转
    if (this->mShouldRotate)
    {
        this->mWorldBox.rotate(this->getActor()->getRotation());
    }

    // 平移
    this->mWorldBox.mMin += this->getActor()->getPosition();
    this->mWorldBox.mMax += this->getActor()->getPosition();
}

AABB& BoxComponent::getObjectBox()
{
    return this->mObjectBox;
}

AABB& BoxComponent::getWorldBox()
{
    return this->mWorldBox;
}

void BoxComponent::setWorldBox(const AABB& box)
{
    this->mWorldBox = box;
}

void BoxComponent::setObjectBox(const AABB& box)
{
    this->mObjectBox = box;
}

void BoxComponent::setShouldRotate(bool shouldRotate)
{
    this->mShouldRotate = shouldRotate;
}

Component::EComponentType BoxComponent::getType() const
{
    return EBoxComponent;
}

void BoxComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);
}

void BoxComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
}