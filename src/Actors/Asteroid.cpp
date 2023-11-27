#include "../../include/Game.h"
#include "../../include/Actors/Asteroid.h"
#include "../../include/Components/SpriteComponent.h"
#include "../../include/Components/MoveComponent.h"
#include "../../include/Components/CircleComponent.h"
#include "../../include/Renderers/Renderer.h"

Asteroid::Asteroid()
{

}

Asteroid::Asteroid(class Game* game) : Actor(game)
{
    // 随机初始化行星的位置和方向
    Vector3 randPos = Random::GetVector(Vector3(-512.0f, -284.0f, 0.0f), Vector3(512.0f, 284.0f, 0.0f));
    setPosition(randPos);
    setRotation(Quaternion(Vector3::UnitZ, Random::GetFloatRange(0.0f, Math::TwoPi)));

    SpriteComponent* sc = new SpriteComponent(this, 110);
    sc->setTexture(game->getRenderer()->getTexture("../../Assets/Images/Asteroid.png"));

    MoveComponent* mc = new MoveComponent(this);
    mc->setForwardSpeed(10.0f);
    mc->setAngularSpeed(Math::Pi / 18);

    CircleComponent* cc = new CircleComponent(this);
    cc->setRadius(40.0f); 

    // this->addComponent(sc);
    // this->getGame()->addSpriteComponent(sc);

    this->mCircleComponent = cc;
    this->mSpriteComponent = sc;
    this->addComponent(mc);
    this->addComponent(cc);

    // game->addActor(this);
    game->addAsteroid(this);
}

Asteroid::~Asteroid()
{

}

CircleComponent* Asteroid::getCircleComponent() const
{
    return this->mCircleComponent;
}

SpriteComponent* Asteroid::getSpriteComponent() const
{
    return this->mSpriteComponent;
}

Actor::EActorType Asteroid::getType() const
{
    return EAsteroid;
}

void Asteroid::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);

}

void Asteroid::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);

}