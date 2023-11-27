#pragma once

#include <stdio.h>

#include "./Actor.h"

class Asteroid : public Actor
{
private:
    class CircleComponent* mCircleComponent;
    class SpriteComponent* mSpriteComponent;

public:
    Asteroid(/* args */);
    Asteroid(class Game* game);

    ~Asteroid();

    class CircleComponent* getCircleComponent() const;
    class SpriteComponent* getSpriteComponent() const override;
    EActorType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};

