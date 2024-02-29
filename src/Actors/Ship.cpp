#include "../../include/Actors/Ship.h"
#include "../../include/Game.h"
#include "../../include/Components/InputComponent.h"
#include "../../include/Components/SpriteComponent.h"
#include "../../include/Components/CircleComponent.h"
#include "../../include/Actors/Laser.h"
#include "../../include/Actors/Asteroid.h"
#include "../../include/Renderers/Renderer.h"

Ship::Ship(class Game* game)
: Actor(game)
{
    mLaserCooldown = 0.0f;

    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->setTexture(game->getRenderer()->getTexture("../../Assets/Images/Ship.png"));

    InputComponent* ic = new InputComponent(this);
    ic->setForwardKey(SDL_SCANCODE_W);
    ic->setBackKey(SDL_SCANCODE_S);
    ic->setClockwiseKey(SDL_SCANCODE_D);
    ic->setCounterClockwiseKey(SDL_SCANCODE_A);
    ic->setMaxForwardSpeed(100.0f);
    ic->setMaxAngularSpeed(Math::Pi * 0.75);

    CircleComponent* cc = new CircleComponent(this);
    cc->setRadius(25.0f);
 
    // this->addComponent(sc);
    // this->getGame()->addSpriteComponent(sc);
    this->addComponent(ic);
    this->addComponent(cc);

    this->mSpriteComponent = sc;
    this->mInputComponent = ic;
    this->mCircleComponent = cc;

    // this->getGame()->addActor(this);
}

Ship::~Ship()
{
    
}

void Ship::updateActor(float dt)
{   
    // 对激光冷却以增量时间递减 
    mLaserCooldown -= dt;
    // this->mInputComponent->update(dt);
    for (auto ast : this->getGame()->getAsteroids())
    {
        if (intersect(ast->getCircleComponent(), this->mCircleComponent))
        {
            // this->getGame()->setRunningState(false);
            SDL_Log("Game over!");
            break;
        }
    }
}

void Ship::actorInput(const uint8_t* keyboard_state)
{
    if (keyboard_state[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
    {   // 若玩家按下空格且激光冷却时间小于0s则发射一条激光
        Laser* laser = new Laser(this->getGame());
        laser->setPosition(this->getPosition());
        laser->setRotation(this->getRotation());

        // 重置激光冷却时间为0.5s
        mLaserCooldown = 0.5f;
    }

    // TODO:处理键盘控制的移动输入按键
}

void Ship::actorHandleMouse(const int& mouse_wheel)
{

}

class SpriteComponent* Ship::getSpriteComponent() const
{
    return mSpriteComponent;    
}

float Ship::getLaserCooldown() const
{
    return this->mLaserCooldown;
}

// void Ship::setPosition(class Vector2 pos)
// {
//     if (pos.x >= 512.0f - this->mCircleComponent->getRadius())
//     {
//         pos.x = 512.0f - this->mCircleComponent->getRadius();
//     }
//     else if (pos.x <= -512.0f + this->mCircleComponent->getRadius())
//     {
//         pos.x = -512.0f + this->mCircleComponent->getRadius();
//     }

//     if (pos.y >= 284.0f - this->mCircleComponent->getRadius())
//     {
//         pos.y = 284.0f - this->mCircleComponent->getRadius();
//     }
//     else if (pos.y <= -284.0f + this->mCircleComponent->getRadius())
//     {
//         pos.y = -284.0f + this->mCircleComponent->getRadius();
//     }
//     this->setFixedPosition(pos);
// }

void Ship::setPosition(Vector3 pos)
{
    
}   

CircleComponent* Ship::getCircleComponent() const
{
    return this->mCircleComponent;
}

Actor::EActorType Ship::getType() const
{
    return EShip;
}

void Ship::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void Ship::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}