#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

class SoundEvent
{
protected:
    friend class AudioSystem;
    SoundEvent(class AudioSystem* system, uint32_t id);

private:
    class AudioSystem* mSystem;
    uint32_t mID;

public:
    SoundEvent(/* args */);
    ~SoundEvent();

    bool isValid();
    void restart();
    void stop(bool allowFadeOut = true);

    // Setters
    void setPaused(bool pause);
    void setVolume(float value);
    void setPitch(float value);
    void setParameter(const std::string& name, float value);

    // Getters
    bool getPaused() const;
    float getVolume() const;
    float getPitch() const;
    float getParameter(const std::string& name);

    bool is3D();
    void set3DAttriutes(const class Matrix4& worldTransform);
};