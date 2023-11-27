#include "../../include/Components/AudioComponent.h"
#include "../../include/Audios/SoundEvent.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Game.h"
#include "../../include/Audios/AudioSystem.h"
#include "../../include/General/LevelLoader.h"

#include "fmod.hpp"
#include "fmod_studio.h"
#include "fmod_common.h"

AudioComponent::AudioComponent(class Actor* owner, int updateOrder)
: Component(owner, updateOrder)
{

}

AudioComponent::~AudioComponent()
{
    this->stopAllEvents();
}

void AudioComponent::update(float dt)
{
    auto iter = this->mEvents2D.begin();
    for (; iter != this->mEvents2D.end(); )
    {
        if (!(*iter).isValid())
        {
            iter = this->mEvents2D.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    iter = this->mEvents3D.begin();
    for (; iter != this->mEvents3D.end(); )
    {
        if (!(*iter).isValid())
        {
            iter = this->mEvents3D.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void AudioComponent::onUpdateWorldTransform()
{
    Matrix4 world = this->getActor()->getWorldTransform();
    for (auto event : this->mEvents3D)
    {
        if (event.isValid())
        {
            event.set3DAttriutes(world);
        }
    }
}

SoundEvent AudioComponent::playEvent(const std::string& name)
{
    SoundEvent e = this->getActor()->getGame()->getAudioSystem()->playEvent(name);
    if (e.is3D())
    {
        this->mEvents3D.emplace_back(e);

        // 设置初始化的3D属性
        e.set3DAttriutes(this->getActor()->getWorldTransform());
    }
    else
    {
        this->mEvents2D.emplace_back(e);
    }
    return e;
}

void AudioComponent::stopAllEvents()
{
    for (auto& e : this->mEvents2D)
    {
        e.stop();
    }

    for (auto& e : this->mEvents3D)
    {
        e.stop();
    }

    this->mEvents2D.clear();
    this->mEvents3D.clear();
}

Component::EComponentType AudioComponent::getType() const
{
    return EAudioComponent;
}

void AudioComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);
}

void AudioComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);

}