#include "./Component.h"

class TargetComponent : public Component
{
private:
    
public:
    TargetComponent(class Actor* owner);
    ~TargetComponent();

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};