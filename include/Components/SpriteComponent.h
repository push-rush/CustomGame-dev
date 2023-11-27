#pragma once
#include <stdlib.h>
#include <stdio.h>
// #include "SDL.h"

#include "./Component.h"

class SpriteComponent : public Component
{
private:
    class Texture* mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;

public:
    SpriteComponent(/* args */);
    SpriteComponent(class Actor* owner, int updateOrder = 100);

    ~SpriteComponent();

    // virtual void draw(SDL_Renderer* renderer);
    virtual void draw(class Shader* shader);
    virtual void setTexture(class Texture* texture);

    int getDrawOrder() const;
    int getTexHeight() const;
    int getTexWidth() const;

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};