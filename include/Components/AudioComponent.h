#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

#include "./Component.h"

class AudioComponent : public Component
{
private:
    std::vector<class SoundEvent> mEvents2D;
    std::vector<class SoundEvent> mEvents3D;

public:
    AudioComponent(class Actor* owner, int updateOrder = 200);
    ~AudioComponent();

    void update(float dt) override;
    void onUpdateWorldTransform() override;

    class SoundEvent playEvent(const std::string& name);
    void stopAllEvents();

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
