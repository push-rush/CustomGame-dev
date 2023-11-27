#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <string.h>
#include <vector>

class AudioSystem
{
private:
    class Game* mGame;
    
    // FMOD音效系统
    class FMOD_STUDIO_SYSTEM* mSystem;
    // 低阶系统
    class FMOD_SYSTEM* mLowLevelSystem;

    std::unordered_map<std::string, class FMOD_STUDIO_BANK*> mBanks;
    std::unordered_map<std::string, class FMOD_STUDIO_EVENTDESCRIPTION*> mEvents;
    std::unordered_map<uint32_t, class FMOD_STUDIO_EVENTINSTANCE*> mEventInstances;
    std::unordered_map<std::string, class FMOD_STUDIO_BUS*> mBuses;
    static uint32_t sNextID;

    void loadBank(const std::string& name);
    void unLoadBank(const std::string& name);
    void unLoadAllBank();
    
public: 
    AudioSystem(class Game* game);
    ~AudioSystem();

    bool init();
    void shutdown();
    void update(float dt);

    class FMOD_STUDIO_EVENTINSTANCE* getEventInstance(uint32_t id);
    class SoundEvent playEvent(const std::string& name);
    void setListener(const class Matrix4& view);

    float getBusVolume(const std::string& name) const;
    bool getBusPaused(const std::string& name) const;

    void setBusVolume(const std::string& name, float value);
    void setBusPaused(const std::string& name, bool paused); 
};