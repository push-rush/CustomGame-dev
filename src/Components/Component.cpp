#include "../../include/Components/Component.h"
#include "../../include/Actors/Actor.h"
#include "../../include/General/LevelLoader.h"

const char* Component::TypeNames[NUM_COMPONENT_TYPES]
{   
    "Component",
    "AudioComponent",
    "BallMove",
    "BoxComponent",
    "CameraComponent",
    "CircleComponent",
    "FollowCamera",
    "FPSCamera",
    "InputComponent",
    "MeshComponent",
    "MirrorCamera",
    "MoveComponent",
    "OrbitCamera",
    "PointLightComponent",
    "SkeletalMeshComponent",
    "SplineCamera",
    "SpriteComponent",
    "TargetComponent",
};

int Component::SCompSequence = 0;

Component::Component()
{
    this->setCompID(Component::SCompSequence);
    ++Component::SCompSequence;
}

Component::Component(class Actor* owner, int updateOrder)
{
    this->setCompID(Component::SCompSequence);
    ++Component::SCompSequence;

    this->mOwner = owner;
    mUpdateOrder = updateOrder;

    this->mOwner->addComponent(this);
}

Component::~Component()
{
    this->mOwner->removeComponent(this);
}

int Component::getUpdateOrder() const
{
    return this->mUpdateOrder;
}

void Component::update(float dt)
{
    
}

void Component::processInput(const uint8_t* keyboard_state)
{
    
}

void Component::handleMouseWheel(const int& mouse_wheel)
{

}

Actor* Component::getActor() const
{
    auto temp = this->mOwner;
    if (temp == nullptr)
    {
        cout << "ggg" << endl;
    }
    return temp;
}

void Component::onUpdateWorldTransform()
{
    
}


Component::EComponentType Component::getType() const
{
    return EComponent;
}

void Component::loadProperties(const rapidjson::Value& inObj)
{
    JsonHelper::getInt(inObj, "updateOrder", this->mUpdateOrder);
}

void Component::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    JsonHelper::addInt(alloc, props, "updateOrder", this->mUpdateOrder);
}

int Component::getCompID() const
{
    return this->mCompID;
}

void Component::setCompID(const int& id)
{
    this->mCompID = id;
}