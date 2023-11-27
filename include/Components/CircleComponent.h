#pragma once
#include "./Component.h"

class CircleComponent : public Component
{
private:
    float mRadius;

public:
    CircleComponent(/* args */);
    CircleComponent(class Actor* owner);
    ~CircleComponent();

    float getRadius() const;
    void setRadius(float radius);
    // Vector2 getCenter();
    Vector3 getCenter();

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};

bool intersect(CircleComponent* cc1, CircleComponent* cc2);