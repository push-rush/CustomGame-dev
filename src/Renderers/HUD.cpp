#include "../../include/Renderers/HUD.h"
#include "../../include/Game.h"
#include "../../include/Components/TargetComponent.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Collision.h"
#include "../../include/Renderers/PhysWorld.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/GBuffer.h"

HUD::HUD(class Game* game)
: UIScreen(game)
{
    this->setUIType(EHUD);

    this->mRadarRange = 2000.0f;
    this->mRadarRadius = 90.0f;
    this->mTargetEnemy = false;

    Renderer* r = game->getRenderer();
    this->mHealthBar = r->getTexture("../Assets/Images/HealthBar.png");
    this->mRadar = r->getTexture("../Assets/Images/Radar.png");
    this->mCrosschair = r->getTexture("../Assets/Images/Crosshair.png");
    this->mCrosshairEnemy = r->getTexture("../Assets/Images/CrosshairRed.png");
    this->mBlipTex = r->getTexture("../Assets/Images/Blip.png");
    this->mRadarArrow = r->getTexture("../Assets/Images/RadarArrow.png");
}

HUD::~HUD()
{

}

void HUD::addTargetComp(class TargetComponent* targetComp)
{
    if (targetComp)
    {
        this->mTargetComponents.emplace_back(targetComp);
    }
}

void HUD::removeTargetComp(class TargetComponent* targetComp)
{
    for (auto iter = mTargetComponents.begin(); iter != mTargetComponents.end();)
    {
        if ((*iter) == targetComp)
        {
            iter = this->mTargetComponents.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void HUD::updateCrosshair(float dt)
{
    this->mTargetEnemy = false;
    const float cAimDist = 5000.0f;

    Vector3 start, dir;
    this->getGame()->getRenderer()->getScreenDirection(start, dir);
    
    LineSegment lineSeg(start, start + dir * cAimDist);
    //线段相交测试
    CollisionInfo info;
    if (this->getGame()->getPhysWorld()->segmentCast(lineSeg, info))
    {
        // 检查对象是否有目标组件
        for (auto tc : this->mTargetComponents)
        {
            if (tc->getActor() == info.mCollActor)
            {
                this->mTargetEnemy = true;
                break;
            }
        }
    }
}

void HUD::draw(class Shader* shader, class Shader* fontShader)
{
    shader->setActive();
    Texture* tex = this->mTargetEnemy ? this->mCrosshairEnemy : this->mCrosschair;
    if (tex)
    {
        this->drawTexture(shader, tex, Vector2::Zero, 1.8f);

        const Vector2 cRadarPos = Vector2(-390.0f, 180.0f);

        // 绘制雷达纹理
        if (this->mRadar)
        {
            this->drawTexture(shader, this->mRadar, cRadarPos, 1.0f);
        }
        else
        {
            SDL_Log("[Radar] Texture is null...");
        }

        for (auto& blip : this->mBlips)
        {   
            if (this->mBlipTex)
            {
                // 绘制光点
                this->drawTexture(shader, this->mBlipTex, cRadarPos + blip, 1.0f);
                // SDL_Log("[HUD] Blip texture...");
            }
            else
            {
                SDL_Log("[Blip] Texture is null...");
            }
        }

        if (this->mRadarArrow)
        {
            this->drawTexture(shader, this->mRadarArrow, cRadarPos);
            // SDL_Log("[HUD] Radar arrow tex...");
        }
        else
        {
            SDL_Log("[HUD] Radar arrow texture is null...");
        }

    }
    else
    {
        SDL_Log("[Crosshair] Texture is null...");
    }

    Texture* mirror = this->getGame()->getRenderer()->getMirrorTexture();
    if (mirror)
    {
        this->drawTexture(shader, mirror, Vector2(-350.0f, -250.0f), 1.0f, 1.0f, true);
        // SDL_Log("[UIScreen] mirror texture...");
    }
    else
    {
        SDL_Log("[UIScreen] Mirror texture is null...");
    }

    // Texture* gbufferTexture = this->getGame()->getRenderer()->getGBuffer()->getTexture(GBuffer::EDiffuse);
    // if (gbufferTexture)
    // {
	//     this->drawTexture(shader, gbufferTexture, Vector2::Zero, 1.0f, true);
    // }
    // else
    // {
    //     SDL_Log("[HUD] GBuffer texture is null...");
    // }
}

void HUD::updateRadar(float dt)
{
    // 每一帧重新计算光点
    this->mBlips.clear();

    Vector3 playerPos = this->getGame()->getPlayer()->getPosition();
    Vector2 playerPos2D = Vector2(playerPos.y, playerPos.x);

    Vector3 playerForward = this->getGame()->getPlayer()->getForward();
    Vector2 playerForward2D = Vector2(playerForward.x , playerForward.y);

    float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
    Matrix3 rotMat = Matrix3::CreateRotation(angle);

    // SDL_Log("[HUD] Target comp size: %d", this->mTargetComponents.size());

    for (auto tc : this->mTargetComponents)
    {
        Vector3 targetPos = tc->getActor()->getPosition();
        Vector2 targetPos2D = Vector2(targetPos.y, targetPos.x);

        Vector2 playerToTarget = targetPos2D - playerPos2D;

        // SDL_Log("[HUD] player pos: (%.3f, %.3f)", playerPos2D.x, playerPos2D.y);
        // SDL_Log("[HUD] target pos: (%.3f, %.3f)", targetPos.x, targetPos.y);
        // SDL_Log("[HUD] player to target pos: (%.3f, %.3f)", playerToTarget.x, playerToTarget.y);

        // 计算目标是否在雷达扫描范围内
        if (playerToTarget.LengthSq() <= (this->mRadarRange * this->mRadarRange))
        {
            Vector2 blipPos = playerToTarget;
            blipPos *= (this->mRadarRadius / this->mRadarRange);

            blipPos = Vector2::Transform(blipPos, rotMat);
            this->mBlips.emplace_back(blipPos);

            // SDL_Log("[HUD] target in range...");
        }
    }

    // SDL_Log("[HUD] Blip size: %d", this->mBlips.size());
}

void HUD::update(float dt)
{
    UIScreen::update(dt);

    this->updateCrosshair(dt);
    this->updateRadar(dt);

    // SDL_Log("[HUD] Update...");
}