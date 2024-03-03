#include "./Actor.h"

class SphereActor : public Actor
{
private:
    class CircleComponent* mCircleComp;

public:
    SphereActor(class Game* game);
    ~SphereActor();

    Actor::EActorType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};

