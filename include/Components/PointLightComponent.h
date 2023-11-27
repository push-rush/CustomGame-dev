#include "./Component.h"

class PointLightComponent : public Component
{
public:
    PointLightComponent(class Actor* owner);
    ~PointLightComponent();

    void draw(class Shader* lightShader, class Mesh* lightMesh);

    Vector3 mDiffuseColor;
    Vector3 mSpecColor;
    float mInnerRadius;
    float mOuterRadius;

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};