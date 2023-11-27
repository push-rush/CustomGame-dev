#include "../../include/Actors/Laser.h"
#include "../../include/Game.h"
#include "../../include/Actors/Asteroid.h"
#include "../../include/Components/CircleComponent.h"
#include "../../include/Components/SpriteComponent.h"
#include "../../include/Components/MoveComponent.h"
#include "../../include/Renderers/Renderer.h"

Laser::Laser(/* args */)
{

}

Laser::Laser(class Game* game)
: Actor(game), mDeathTime(1.0f)
{
    // 通过精灵组件添加激光纹理
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(game->getRenderer()->getTexture("../../Assets/Images/Laser.png"));

    // 通过移动组件赋予激光前向速度
    MoveComponent* mc = new MoveComponent(this);
    mc->setForwardSpeed(568.0f);
    mc->setAngularSpeed(0.0f);

    // 创建圆组件来进行碰撞检测
    CircleComponent* cc = new CircleComponent(this);
    cc->setRadius(8.0f);

    // this->addComponent(sc);
    // this->getGame()->addSpriteComponent(sc);
    // this->getGame()->addSpriteComponent(sc);
    this->mCircleComponent = cc;
    this->mSpriteComponent = sc;
    this->addComponent(mc);
    this->addComponent(cc);

    // this->getGame()->addActor(this);
    this->getGame()->addLaser(this);
}

Laser::~Laser()
{
}

void Laser::updateActor(float dt)
{
    mDeathTime -= dt;

    if (mDeathTime <= 0.0f)
    {
        this->setState(EDead);
    }
    else
    {
        auto vec = this->getGame()->getAsteroids();
        for (auto ast : vec)
        {
            if (intersect((ast->getCircleComponent()), (this->mCircleComponent)))
            {   // 如果激光和小行星相交
                this->setState(EDead);
                ast->setState(EDead);
                break;
            }
        }
    }
}

SpriteComponent* Laser::getSpriteComponent() const
{
    return this->mSpriteComponent;
}

Actor::EActorType Laser::getType() const
{
    return ELaser;
}

void Laser::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void Laser::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}