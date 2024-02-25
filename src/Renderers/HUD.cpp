#include "../../include/Renderers/HUD.h"

#include "../../include/Game.h"

#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/GBuffer.h"
#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/ResourceMenu.h"
#include "../../include/Renderers/Console.h"

#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/Collision.h"
#include "../../include/Renderers/PhysWorld.h"

#include "../../include/Components/TargetComponent.h"

#include "../../include/General/ResourceManager.h"


HUD::HUD(class Game* game)
: UIScreen(game)
{
    this->setUIType(EHUD);

    this->mRadarRange = 2000.0f;
    this->mRadarRadius = 90.0f;
    this->mTargetEnemy = false;

    this->mRadar = nullptr;
    this->mHealthBar = nullptr;
    this->mBlipTex = nullptr;
    this->mCrosschair = nullptr;
    this->mCrosshairEnemy = nullptr;
    this->mRadarArrow = nullptr;

    // Renderer* r = game->getRenderer();
    // this->mHealthBar = r->getTexture("../Assets/Images/HealthBar.png");
    // this->mRadar = r->getTexture("../Assets/Images/Radar.png");
    // this->mCrosschair = r->getTexture("../Assets/Images/Crosshair.png");
    // this->mCrosshairEnemy = r->getTexture("../Assets/Images/CrosshairRed.png");
    // this->mBlipTex = r->getTexture("../Assets/Images/Blip.png");
    // this->mRadarArrow = r->getTexture("../Assets/Images/RadarArrow.png");
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

void HUD::draw(class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader, class EmptySprite* elem)
{
    float w = this->getGame()->getRenderer()->getScreenWidth();
    float h = this->getGame()->getRenderer()->getScreenHeight();

    // 绘制后视镜
    class Texture* mirror = this->getGame()->getRenderer()->getMirrorTexture();
    this->getSpriteVerts()->setActive();
    auto buffer_pos = this->getUIBufferPos() + this->getUIPosOffset() - Vector2{w * 0.5f, h * 0.5f};
    auto mir_offset = Vector2{(float)(mirror->getWidth()) * 0.60f, (float)(mirror->getHeight()) * 0.55f};
    this->drawTexture(
        spriteShader, 
        mirror, 
        buffer_pos + mir_offset, 
        1.0f, 1.0f, true
    );

    for (auto elem : this->getUIElements())
    {
        UIScreen::draw(nullptr, spriteShader, fontShader, elem);

        if (!strcmp(elem->getSpriteName().c_str(), "Radar"))
        {
            EmptySprite* radar = elem;
            // SDL_Log("[HUD] Radar pos: [%.3f %.3f]", radar->getPosition().x, radar->getPosition().y);
            
            auto iter = this->getUITextures().find("blipTex");
            if (iter != this->getUITextures().end())
            {
                auto tex = (*iter).second;
                if (tex)
                {
                    for (auto& blip : this->mBlips)
                    {   
                        // 绘制光点
                        this->getSpriteVerts()->setActive();
                        this->drawTexture(spriteShader, tex, radar->getPosition() + blip, 1.0f);
                        // SDL_Log("[HUD] Blip texture...");
                    }
                }
            }
        }
    }

    // 绘制ui树节点
    if ((int)(this->mNodeStack.size()) > 0)
    {
        for (auto node : this->mNodeStack)
        {
            UIScreen::draw(basicShader, spriteShader, fontShader, reinterpret_cast<EmptySprite*>(node->mNodeValuePointer));
        }
    }
}

// void HUD::draw(class Shader* shader, class Shader* fontShader, EmptySprite* e, const Vector2& offset)
// {
//     // UIScreen::draw(shader, fontShader, this->getUIElements());
    
//     for (auto elem : this->getUIElements())
//     {
//         UIScreen::draw(shader, fontShader, elem);

//         if (!strcmp(elem->getSpriteName().c_str(), "Radar"))
//         {
//             EmptySprite* radar = elem;
//             // SDL_Log("[HUD] Radar pos: [%.3f %.3f]", radar->getPosition().x, radar->getPosition().y);
            
//             auto iter = this->getUITextures().find("blipTex");
//             if (iter != this->getUITextures().end())
//             {
//                 auto tex = (*iter).second;
//                 if (tex)
//                 {
//                     for (auto& blip : this->mBlips)
//                     {   
//                         // 绘制光点
//                         this->drawTexture(shader, tex, radar->getPosition() + blip, 1.0f);
//                         // SDL_Log("[HUD] Blip texture...");
//                     }
//                 }
//             }
//         }
//     }

//     // 绘制ui树节点
//     if ((int)(this->mNodeStack.size()) > 0)
//     {
//         for (auto node : this->mNodeStack)
//         {
//             UIScreen::draw(shader, fontShader, reinterpret_cast<EmptySprite*>(node->mNodeValuePointer));
//         }
//     }
    
//     // 绘制资源管理ui视图
//     // auto resource_manager = this->getGame()->getResourceManager();
//     // if (resource_manager)
//     // {
//     //     TreeNode* node = resource_manager->getResourceTree().findTreeNode("root");

//     // }

// //     // std::vector<UIElement*> vec;
// //     // for (auto iter = elems.begin(); iter != elems.end(); ++iter)
// //     // {
// //     //     vec.emplace_back((*iter).second);
// //     // }    

// //     // sort(vec.begin(), vec.end(), [](UIElement* e1, UIElement* e2)
// //     // {
// //     //     return e1->mUpdateOrder < e2->mUpdateOrder;
// //     // });

// //     // for (auto e : vec)
// //     // {
// //     //     if (e->mTexture)
// //     //     {
// //     //         // SDL_Log("[HUD] tex updateOrder: %d", e->mUpdateOrder);
// //     //         this->drawTexture(shader, e->mTexture, e->mPosOffset);
// //     //     }
// //     // }

// //     if (!this->mRadar)
// //     {
// //         auto elems = this->getUIElements();

// //         this->mRadar = (*elems.find("Radar")).second;
// //         this->mCrosschair = (*elems.find("Crosschair")).second;     
// //         this->mCrosshairEnemy = (*elems.find("CrosshairEnemy")).second;
// //         this->mBlipTex = (*elems.find("BlipTex")).second;
// //         this->mRadarArrow = (*elems.find("RadarArrow")).second;
// //     }

// //     UIScreen::UIElement* uiCrosshair = this->mTargetEnemy ? this->mCrosshairEnemy : this->mCrosschair;
// //     if (uiCrosshair->mTexture)
// //     {
// //         this->drawTexture(shader, uiCrosshair->mTexture, uiCrosshair->mPosOffset, 1.8f);

// //         // const Vector2 cRadarPos = Vector2(-390.0f, 180.0f);
// //         // 绘制雷达纹理
// //         if (this->mRadar->mTexture)
// //         {
// //             this->drawTexture(shader, this->mRadar->mTexture, this->mRadar->mPosOffset, 1.0f);
// //         }
// //         else
// //         {
// //             SDL_Log("[Radar] Texture is null...");
// //         }

// //         for (auto& blip : this->mBlips)
// //         {   
// //             if (this->mBlipTex->mTexture)
// //             {
// //                 // 绘制光点
// //                 this->drawTexture(shader, this->mBlipTex->mTexture, this->mRadar->mPosOffset + blip, 1.0f);
// //                 // SDL_Log("[HUD] Blip texture...");
// //             }
// //             else
// //             {
// //                 SDL_Log("[Blip] Texture is null...");
// //             }
// //         }
        
// //         if (this->mRadarArrow->mTexture)
// //         {
// //             this->drawTexture(shader, this->mRadarArrow->mTexture, uiRadarArrow->mPosOffset);
// //             // SDL_Log("[HUD] Radar arrow tex...");
// //         }
// //         else
// //         {
// //             SDL_Log("[HUD] Radar arrow texture is null...");
// //         }

// //     }
// //     else
// //     {
// //         SDL_Log("[Crosshair] Texture is null...");
// //     }

// //     Texture* mirror = this->getGame()->getRenderer()->getMirrorTexture();
// //     if (mirror)
// //     {
// //         this->drawTexture(shader, mirror, Vector2(-350.0f, -250.0f), 1.0f, 1.0f, true);
// //         // SDL_Log("[UIScreen] mirror texture...");
// //     }
// //     else
// //     {
// //         SDL_Log("[UIScreen] Mirror texture is null...");
// //     }

// //     // Texture* gbufferTexture = this->getGame()->getRenderer()->getGBuffer()->getTexture(GBuffer::EDiffuse);
// //     // if (gbufferTexture)
// //     // {
// // 	//     this->drawTexture(shader, gbufferTexture, Vector2::Zero, 1.0f, true);
// //     // }
// //     // else
// //     // {
// //     //     SDL_Log("[HUD] GBuffer texture is null...");
// //     // }
// }

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
    // SDL_Log("[HUD] Update st...");

    UIScreen::update(dt);

    if (this->getGame()->getResourceMenu()->getState() == EActive || 
        this->getGame()->getResourceMenu()->getState() == EVisible
    )
    {
        float scale_x = this->getGame()->getResourceMenu()->getUIViewScale().x;
        float scale_y = this->getGame()->getConsole()->getUIViewScale().y;
        float h = this->getGame()->getRenderer()->getScreenHeight();
        this->setUIViewScale(Vector2{1.0f - scale_x, 1.0f - scale_y});
        this->setUIPosOffset(Vector2{
            -this->getUIBufferPos().x, 
            h - this->getUIBufferPos().y - this->getUIBufferViewSize().y
        });
    }
    
    // SDL_Log("[HUD] Update mid1...");

    this->updateCrosshair(dt);

    // SDL_Log("[HUD] Update mid2...");

    this->updateRadar(dt);

    // SDL_Log("[HUD] Update over...");
}

void HUD::bindEvent(const UIBindEvent& event, class Button* b)
{
    SDL_Log("[HUD] Bind event id: %d", (int)(event));

    UIScreen::bindEvent(event, b);

    switch (event)
    {
        case UIQuit:
        {
            this->close();
            break;
        }
        case OpenMenuLayer1:
        {
            SDL_Log("[HUD] Click layer 1...");

            TreeNode* root = this->mMenuTree.findTreeNode("root");
            bool flag = false;
            if (root)
            {
                auto n = (*root->mChildren.begin());
                for (auto s : this->mNodeStack)
                {
                    if (!strcmp(n->mNodeName.c_str(), s->mNodeName.c_str()))
                    {
                        flag = true;
                        break;
                    }
                }
            }

            if (flag)
            {
                SDL_Log("[HUD] Close layer1...");

                for (auto n : root->mChildren)
                {
                    for (auto iter = this->mNodeStack.begin() + 1; iter != this->mNodeStack.end();)
                    {
                        if (!strcmp((*iter)->mNodeName.c_str(), n->mNodeName.c_str()))
                        {
                            iter = this->mNodeStack.erase(iter);
                            break;
                        }
                        else
                        {
                            ++iter;
                        }
                    }
                }
            }
            else
            {
                SDL_Log("[HUD] Open layer1...");

                for (auto n : root->mChildren)
                {
                    this->mNodeStack.emplace_back(n);
                }
            }
            break;
        }
        case OpenMenuLayer2_1:
        {
            TreeNode* layer1 = this->mMenuTree.findTreeNode("MenuLayer1_2");
            bool flag = false;
            if (layer1)
            {
                auto n = (*layer1->mChildren.begin());
                for (auto s : this->mNodeStack)
                {
                    if (!strcmp(n->mNodeName.c_str(), s->mNodeName.c_str()))
                    {
                        flag = true;
                        break;
                    }
                }
            }

            if (flag)
            {
                SDL_Log("[HUD] Close layer2_1...");

                for (auto n : layer1->mChildren)
                {
                    for (auto iter = this->mNodeStack.begin() + 1; iter != this->mNodeStack.end();)
                    {
                        if (!strcmp((*iter)->mNodeName.c_str(), n->mNodeName.c_str()))
                        {
                            iter = this->mNodeStack.erase(iter);
                            break;
                        }
                        else
                        {
                            ++iter;
                        }
                    }
                }
            }
            else
            {
                SDL_Log("[HUD] Open layer2_1...");
                
                for (auto n : layer1->mChildren)
                {
                    this->mNodeStack.emplace_back(n);
                }
            }

            break;
        }
        default:
            break;
    }
}

TreeStruct* HUD::getUIMenuTree() 
{
    return (&this->mMenuTree);
}

void HUD::handleKeyPress(int key)
{
    switch (key)
    {
    case (SDL_BUTTON_LEFT):
    {
        if (!this->getUIElements().empty())
        {
            for (auto elem : this->getUIElements())
            {
                if (!strcmp(elem->getType().c_str(), "button"))
                {
                    auto b = (Button*)(elem);
                    if (b && b->getHighlighted())
                    {
                        b->onClick();
                        // break;
                    }
                }
            }
        }

        if (!this->mNodeStack.empty())
        {
            for (auto n : this->mNodeStack)
            {
                 if (!strcmp((reinterpret_cast<EmptySprite*>(n->mNodeValuePointer))->getType().c_str(), "button"))
                {
                    auto b = (Button*)(n->mNodeValuePointer);
                    if (b && b->getHighlighted())
                    {
                        b->onClick();
                    }
                }
            }
        }
        break;
    }
    case (SDLK_SPACE):
    {
        // 设置为运行态
        this->getGame()->setGameState(Game::EGamePlay);

        // this->setUIState(EInvisible);

        // 设置鼠标为相对模式
        this->setRelativeMouseMode(true);
        
        break;
    }
    case (SDLK_ESCAPE):
    {
        // this->close();
        break;
    }
    default:
        break;
    }
}

void HUD::nodeAddToStack(TreeNode* node)
{
    if (node)
    {
        this->mNodeStack.emplace_back(node);
    }
}   

void HUD::processInput(const uint8_t* keys)
{
    if (!this->getUIElements().empty())
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
        mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
        mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

        for (auto elem : this->getUIElements())
        {
            if (!strcmp(elem->getType().c_str(), "button"))
            {
                Button* b = reinterpret_cast<Button*>(elem);
                if (b->containsPoint(mousePos))
                {
                    b->setHighlighted(true);
                    // SDL_Log("[UIScreen] button: %s is highlighted...", b->getSpriteName().c_str());
                }
                else
                {
                    b->setHighlighted(false);
                }

                // SDL_Log("[UIScreen] button STATE: %d MOUSE pos: (%d, %d) button pos: (%.2f, %.2f)", 
                    // (int)b->getHighlighted(), x, y, b->getPosition().x, b->getPosition().y);
            }
        }

        if (!this->mNodeStack.empty())
        {
            for (auto n : this->mNodeStack)
            {
                if (!strcmp((reinterpret_cast<EmptySprite*>(n->mNodeValuePointer))->getType().c_str(), "button"))
                {
                    Button* b = reinterpret_cast<Button*>(n->mNodeValuePointer);
                    if (b->containsPoint(mousePos))
                    {
                        b->setHighlighted(true);
                        // SDL_Log("[UIScreen] button: %s is highlighted...", b->getSpriteName().c_str());
                    }
                    else
                    {
                        b->setHighlighted(false);
                    }
                    // SDL_Log("[UIScreen] button STATE: %d MOUSE pos: (%d, %d) button pos: (%.2f, %.2f)", 
                        // (int)b->getHighlighted(), x, y, b->getPosition().x, b->getPosition().y);
                }
            }
        }
    }
}