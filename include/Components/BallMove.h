#include "./MoveComponent.h"

class BallMove : public MoveComponent
{
private:
    class Actor* mPlayer;

public:
    BallMove(class Actor* owner);
    ~BallMove();

    void update(float dt) override;
    void setPlayer(class Actor* player);

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};