#include "../../include/Audios/AudioSystem.h"

#include <SDL2/SDL.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.h>
#include <fmod_common.h>

#include "../../include/Audios/SoundEvent.h"
#include "../../include/General/Cmath.h"

uint32_t AudioSystem::sNextID = 0;

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

AudioSystem::AudioSystem(class Game* game)
{
    this->mGame = game;
    this->mLowLevelSystem = nullptr;
    this->mSystem = nullptr;
}

AudioSystem::~AudioSystem()
{

}

bool AudioSystem::init()
{
    FMOD::Debug_Initialize(
        FMOD_DEBUG_LEVEL_ERROR,  // 仅记录错误日志
        FMOD_DEBUG_MODE_TTY      // 日志写入标准输出
    );

    // 创建FMOD Studio系统
    FMOD_RESULT result;
    result = FMOD_Studio_System_Create(&this->mSystem, FMOD_VERSION);
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }

    // 初始化
    result = FMOD_Studio_System_Initialize(
        this->mSystem,
        512,
        FMOD_STUDIO_INIT_NORMAL,
        FMOD_INIT_CHANNEL_LOWPASS,
        nullptr
    );
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to init FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }

    FMOD_Studio_System_GetCoreSystem(this->mSystem, &this->mLowLevelSystem);

    this->loadBank("../Assets/Audios/Master Bank.strings copy.bank");
    this->loadBank("../Assets/Audios/Master Bank copy.bank");
    return true;
}

void AudioSystem::shutdown()
{
    this->unLoadAllBank();
    if (this->mSystem)
    {
        FMOD_Studio_System_Release(this->mSystem);
    }
}

void AudioSystem::update(float dt)
{
    // 找到所有已经停止的事件
    std::vector<uint32_t> stopped_instances;
    for (auto& iter : this->mEventInstances)
    {
        FMOD_STUDIO_EVENTINSTANCE* event = iter.second;
        FMOD_STUDIO_PLAYBACK_STATE state;
        FMOD_Studio_EventInstance_GetPlaybackState(event, &state);
        if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
        {
            FMOD_Studio_EventInstance_Release(event);
            stopped_instances.emplace_back(iter.first);
        }
    }

    // 从map中删除事件实例
    for (auto id : stopped_instances)
    {
        this->mEventInstances.erase(id);
    }

    // 更新FMOD
    FMOD_Studio_System_Update(this->mSystem);
}

void AudioSystem::loadBank(const std::string& name)
{
    // 确定此存储库是否已经加载
    if (this->mBanks.find(name) != mBanks.end())
    {
        return;
    }

    // 尝试加载bank
    FMOD_STUDIO_BANK* bank = nullptr;
    FMOD_RESULT result = FMOD_Studio_System_LoadBankFile(
        this->mSystem, 
        name.c_str(),
        FMOD_STUDIO_LOAD_BANK_NORMAL,
        &bank
    );

    const int maxPathLength = 512;
    if (result == FMOD_OK)
    {
        this->mBanks.emplace(name, bank);
        FMOD_Studio_Bank_LoadSampleData(bank);
        int numEvents = 0;
        FMOD_Studio_Bank_GetEventCount(bank, &numEvents);
        if (numEvents > 0)
        {
            // 得到存储库中事件描述列表
            std::vector<FMOD_STUDIO_EVENTDESCRIPTION*> events(numEvents);
            FMOD_Studio_Bank_GetEventList(bank, events.data(), numEvents, &numEvents);
            char eventName[maxPathLength];
            for (int i = 0; i < numEvents; ++i)
            {
                FMOD_STUDIO_EVENTDESCRIPTION* e = events[i];
                // 得到事件路径
                FMOD_Studio_EventDescription_GetPath(e, eventName, maxPathLength, nullptr);
                this->mEvents.emplace(eventName, e);
            }
        }

        int numBuses = 0;
        FMOD_Studio_Bank_GetBusCount(bank, &numBuses);
        if (numBuses > 0)
        {
            std::vector<FMOD_STUDIO_BUS*> buses(numBuses);
            FMOD_Studio_Bank_GetBusList(bank, buses.data(), numBuses, &numBuses);
            char busName[maxPathLength];
            for (int i = 0; i < numBuses; ++i)
            {
                FMOD_STUDIO_BUS* b = buses[i];
                FMOD_Studio_Bus_GetPath(b, busName, maxPathLength, nullptr);
                this->mBuses.emplace(busName, b);
            }
        }
    }
}

void AudioSystem::unLoadBank(const std::string& name)
{
    auto iter = this->mBanks.find(name);
    if (iter == this->mBanks.end())
    {
        return;
    }

    const int maxPathLength = 512;
    FMOD_STUDIO_BANK* bank = iter->second;
    
    int numEvents = 0;
    FMOD_Studio_Bank_GetEventCount(bank, &numEvents);
    if (numEvents > 0)
    {
        char eventName[maxPathLength];
        std::vector<FMOD_STUDIO_EVENTDESCRIPTION*> events(numEvents);
        FMOD_Studio_Bank_GetEventList(bank, events.data(), numEvents, &numEvents);
        for (int i = 0; i < numEvents; ++i)
        {
            FMOD_STUDIO_EVENTDESCRIPTION* e = events[i];
            FMOD_Studio_EventDescription_GetPath(e, eventName, maxPathLength, nullptr);
            auto event_iter = this->mEvents.find(eventName);
            if (event_iter != this->mEvents.end())
            {
                event_iter = this->mEvents.erase(event_iter);
            }
        }

    }

    int numBuses = 0;
    FMOD_Studio_Bank_GetBusCount(bank, &numBuses);
    if (numBuses)
    {
        char BusName[maxPathLength];
        std::vector<FMOD_STUDIO_BUS*> buses(numBuses);
        FMOD_Studio_Bank_GetBusList(bank, buses.data(), numBuses, &numBuses);
        for (int i = 0; i < numBuses; ++i)
        {
            FMOD_STUDIO_BUS* b = buses[i];
            FMOD_Studio_Bus_GetPath(b, BusName, maxPathLength, nullptr);
            auto bus_iter = this->mBuses.find(BusName);
            if (bus_iter != this->mBuses.end())
            {
                bus_iter = this->mBuses.erase(bus_iter);
            }
        }
    }

    // 删除存储库
    FMOD_Studio_Bank_UnloadSampleData(bank);
    FMOD_Studio_Bank_Unload(bank);
    this->mBanks.erase(iter);
}

void AudioSystem::unLoadAllBank()
{
    for (auto& bank : this->mBanks)
    {
        FMOD_Studio_Bank_UnloadSampleData(bank.second);
        FMOD_Studio_Bank_Unload(bank.second);
    }
    this->mBanks.clear();
    this->mEvents.clear();
}

SoundEvent AudioSystem::playEvent(const std::string& name)
{
    uint32_t retId = 0;
    auto iter = this->mEvents.find(name);
    if (iter != this->mEvents.end())
    {
        // 创建事件实例
        FMOD_STUDIO_EVENTINSTANCE* event = nullptr;
        FMOD_Studio_EventDescription_CreateInstance((*iter).second, &event);
        if (event)
        {
            // 启动事件实例
            FMOD_Studio_EventInstance_Start(event);

            sNextID++;
            retId = sNextID;
            this->mEventInstances.emplace(retId, event);

            // 当事件停止后释放
            // FMOD_Studio_EventInstance_Release(event);
        }
    }
    else
    {
        SDL_Log("[AudioSystem] Event is null...");
    }
    return SoundEvent(this, retId);
}

FMOD_STUDIO_EVENTINSTANCE* AudioSystem::getEventInstance(uint32_t id)
{
    FMOD_STUDIO_EVENTINSTANCE* event = nullptr;
    auto iter = this->mEventInstances.find(id);
    if (iter != this->mEventInstances.end())
    {
        event = (*iter).second;
    }
    return event;
}

void AudioSystem::setListener(const Matrix4& view)
{
    // 转置
    Matrix4 invView = view;
    invView.Invert();
 
    FMOD_3D_ATTRIBUTES listener;
    // 设置侦听器位置、向前向量、向上向量
    listener.position = vecToFMOD(invView.GetTranslation());
    listener.forward = vecToFMOD(invView.GetZAxis());
    listener.up = vecToFMOD(invView.GetYAxis());
    // 多普勒效应
    listener.velocity = vecToFMOD(Vector3(0.0, 0.0, 0.0));

    FMOD_Studio_System_SetListenerAttributes(this->mSystem, 0, &listener, nullptr);
}

float AudioSystem::getBusVolume(const std::string& name) const
{
    float volume = 0.0f;
    auto iter = this->mBuses.find(name);
    if (iter != this->mBuses.end())
    {
        FMOD_Studio_Bus_GetVolume((*iter).second, &volume, &volume);
    }
    return volume;
}

bool AudioSystem::getBusPaused(const std::string& name) const
{
    FMOD_BOOL paused = false;
    auto iter = this->mBuses.find(name);
    if (iter != this->mBuses.end())
    {
        FMOD_Studio_Bus_GetPaused((*iter).second, &paused);
    }
    return paused;
}   

void AudioSystem::setBusVolume(const std::string& name, float value)
{
    auto iter = this->mBuses.find(name);
    if (iter != this->mBuses.end())
    {
        FMOD_Studio_Bus_SetVolume((*iter).second, value);
    }
}

void AudioSystem::setBusPaused(const std::string& name, bool paused)
{
    auto iter = this->mBuses.find(name);
    if (iter != this->mBuses.end())
    {
        FMOD_Studio_Bus_SetPaused((*iter).second, paused);
    }
}