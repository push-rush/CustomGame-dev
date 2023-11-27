#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>

#include "./Component.h"

class CameraComponent : public Component
{
private:
    
public:
    CameraComponent(class Actor* owner);
    ~CameraComponent();

    void setViewMatrix(const Matrix4& view);

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};