#include "./Component.h"
#include "./Renderers/Collision.h"

class BoxComponent : public Component
{
private:
    class AABB mObjectBox;
    class AABB mWorldBox;
    bool mShouldRotate;

public:
    BoxComponent(class Actor* owner);
    ~BoxComponent();

    void onUpdateWorldTransform() override;

    // getters
    AABB& getObjectBox();
    AABB& getWorldBox();

    // setters
    void setWorldBox(const AABB& box);
    void setObjectBox(const AABB& box);
    void setShouldRotate(bool);

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};