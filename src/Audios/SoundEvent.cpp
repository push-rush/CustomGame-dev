#include "../../include/Audios/SoundEvent.h"
#include "../../include/Audios/AudioSystem.h"
#include "../../include/General/Cmath.h"

#include "fmod.h"
#include "fmod_errors.h"
#include "fmod_studio.h"
#include "SDL.h"

namespace
{
    FMOD_VECTOR vecToFMOD(const Vector3& in)
    {
        FMOD_VECTOR v;
        v.x = in.y;
        v.y = in.z;
        v.z = in.x;

        return v;
    }
}

SoundEvent::SoundEvent(/* args */)
: mSystem(nullptr), mID(0)
{

}

SoundEvent::SoundEvent(AudioSystem* system, uint32_t id)
{
    this->mSystem = system;
    this->mID = id;
}

SoundEvent::~SoundEvent()
{

}

bool SoundEvent::isValid()
{
    return (this->mSystem && this->mSystem->getEventInstance(this->mID) != nullptr);
}

void SoundEvent::restart()
{
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_Start(event);
    }
    else
    {
        SDL_Log("[SoundEvent] event is null...");
    }
}

void SoundEvent::stop(bool allowFadeOut)
{
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_STUDIO_STOP_MODE mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
        FMOD_Studio_EventInstance_Stop(event, mode);
    }
}

// Setters
void SoundEvent::setPaused(bool pause)
{
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_SetPaused(event, pause);
    }
    else
    {
        SDL_Log("[SoundEvent] event is null...");
    }
}

void SoundEvent::setVolume(float value)
{
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_SetVolume(event, value);
    }
}

void SoundEvent::setPitch(float value)
{
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_SetPitch(event, value);
    }
}

void SoundEvent::setParameter(const std::string& name, float value)
{
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_SetParameterValue(event, name.c_str(), value);
    }
}

// Getters
bool SoundEvent::getPaused() const
{
    FMOD_BOOL paused = false;
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_GetPaused(event, &paused);
    }
    return paused;
}

float SoundEvent::getVolume() const
{
    float volume = 0.0f;
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_GetVolume(event, &volume, &volume);
    }
    return volume;
}

float SoundEvent::getPitch() const
{
    float pitch = 0.0f;
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_GetPitch(event, &pitch, &pitch);
    }
    return pitch;
}

float SoundEvent::getParameter(const std::string& name)
{
    float value = 0.0f;
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_Studio_EventInstance_GetParameterValue(event, name.c_str(), &value, &value);
    }
    return value;
}

bool SoundEvent::is3D()
{
    FMOD_BOOL retVal = false;
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_STUDIO_EVENTDESCRIPTION* ed = nullptr;
        FMOD_Studio_EventInstance_GetDescription(event, &ed);
        if (ed)
        {
            FMOD_Studio_EventDescription_Is3D(ed, &retVal);
        }
    }
    return retVal;
}

void SoundEvent::set3DAttriutes(const Matrix4& worldTransform)
{
    auto event = this->mSystem ? this->mSystem->getEventInstance(this->mID) : nullptr;
    if (event)
    {
        FMOD_3D_ATTRIBUTES attr;
        // 设置3D位置、前向方向、向上方向
        attr.position = vecToFMOD(worldTransform.GetTranslation());
        attr.forward = vecToFMOD(worldTransform.GetXAxis());
        attr.up = vecToFMOD(worldTransform.GetZAxis());
        attr.velocity = vecToFMOD(Vector3(0.0f, 0.0f, 0.0f));

        FMOD_Studio_EventInstance_Set3DAttributes(event, &attr);
    }
}
