#include "../../include/Renderers/ResourceMenu.h"
#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/TextButton.h"

#include "../../include/General/Cmath.h"

#include "../../include/Game.h"

ResourceMenu::ResourceMenu(class Game* game)
: UIScreen(game), mFlag(false)
{
    this->setUIType(EResourceMenu);
    
    this->mLeftButtonPos = Vector2{0.0f, 0.0f};
    this->mBottomButtonPos = Vector2{0.0f, 0.0f};
}

ResourceMenu::~ResourceMenu()
{

}

void ResourceMenu::draw(
    class Shader* basicShader, 
    class Shader* spriteShader, 
    class Shader* fontShader,
    class EmptySprite* e
)
{
    for (auto elem : this->getUIElements())
    {
        if (!strcmp(elem->getSpriteName().c_str(), "left_border") ||
            !strcmp(elem->getSpriteName().c_str(), "bottom_border") ||
            !strcmp(elem->getSpriteName().c_str(), "right_border")
        )
        {
            // SDL_Log("[ResourceMenu] Border button: %s", elem->getSpriteName().c_str());

            Button temp = *((Button*)(elem));
            temp.setPosition(temp.getPosition() - this->getUITranslation());
            UIScreen::draw(basicShader, spriteShader, fontShader, (EmptySprite*)(&temp)); 
        }
        else
        {
            UIScreen::draw(basicShader, spriteShader, fontShader, elem); 
        }
    }

    // 绘制ui树节点
    if ((int)(this->mNodeStack.size()) > 0)
    {
        for (auto node : this->mNodeStack)
        {
            if (((EmptySprite*)node->mNodeValuePointer)->getSpriteState() == EmptySprite::EDisplay)
            {
                UIScreen::draw(basicShader, spriteShader, fontShader, (EmptySprite*)(node->mNodeValuePointer));
            } 
        }
    }

    // 绘制资源ui树
    if (((int)this->mResourceStack.size()) > 0)
    {
        // std::cout << "stack size:" << (int)this->mResourceStack.size() << std::endl;
        for (auto node : this->mResourceStack)
        {
            auto resource_manager = this->getGame()->getResourceManager();
            auto n = resource_manager->getResourceTree().findTreeNode(node->mNodeName);

            if (n && ((EmptySprite*)node->mNodeValuePointer)->getSpriteState() == EmptySprite::EDisplay &&
                ((ResourceManager::ResourceProperty*)(n->mNodeValuePointer))->mState == ResourceManager::EDisplay
            )
            {
                UIScreen::draw(basicShader, spriteShader, fontShader, (EmptySprite*)(node->mNodeValuePointer));
            }
        }
    }

    auto r = this->mResourceEventTree.findTreeNode("root");
    if (r && r->mNodeValuePointer && (int)(this->mResourceEventStack.size()) > 0)
    {
        for (auto n : this->mResourceEventStack)
        {
            auto b = (Button*)(n->mNodeValuePointer);
            if (b->getSpriteState() == EmptySprite::EDisplay)
            {
                UIScreen::draw(basicShader, spriteShader, fontShader, b);
            }
        }
    }
}

void ResourceMenu::update(float dt)
{
    UIScreen::update(dt);

    Button *left_border = nullptr, *bottom_border = nullptr, *right_border = nullptr;
    for (auto elem : this->getUIElements())
    {
        if (!strcmp(elem->getSpriteName().c_str(), "left_border"))
        {   
            left_border = (Button*)(elem);
        }
        else if (!strcmp(elem->getSpriteName().c_str(), "bottom_border"))
        {
            bottom_border = (Button*)(elem);
        }
        else if (!strcmp(elem->getSpriteName().c_str(), "right_border"))
        {
            right_border = (Button*)(elem);
        }
    }

    int x = 0, y = 0;
    Uint32 mouse = SDL_GetMouseState(&x, &y);
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    Vector2 relative_pos = Vector2{mousePos.x - (this->getGame()->getRenderer()->getScreenWidth() * 0.5f),
        -mousePos.y + (this->getGame()->getRenderer()->getScreenHeight() * 0.5f)
    };

    // SDL_Event event;
    // int mouse_wheel = 0;
    // while (SDL_PollEvent(&event))
    // {
    //     if (event.type == SDL_MOUSEWHEEL)
    //     {
    //         mouse_wheel = event.wheel.y;
    //         break;
    //     }
    // }
 
    bool flag = false;
    if ((mouse & SDL_BUTTON(SDL_BUTTON_LEFT)))
    {
        // SDL_Log("[ResourceMenu] click pos: %d %d", (int)mousePos.x, (int)mousePos.y);

        float w = this->getGame()->getRenderer()->getScreenWidth();
        float h = this->getGame()->getRenderer()->getScreenHeight();

        if (abs(int(relative_pos.x - left_border->getPosition().x)) < 20.0f && 
            relative_pos.y > (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f)
        )
        {
            Vector2 scale = Vector2{(w - mousePos.x) / w, this->getUIViewScale().y};
            this->setUIViewScale(scale);

            left_border->setPosition(Vector2{relative_pos.x, left_border->getPosition().y});

            bottom_border->setPosition(Vector2{
                this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + this->getUIBufferViewSize().x * 0.5f, 
                bottom_border->getPosition().y
            });
            bottom_border->setDimension(Vector2{this->getUIBufferViewSize().x, 20.0f});

            // mLeftButtonPos = left_border->getPosition();
            // mBottomButtonPos = bottom_border->getPosition();
            // mRightButtonPos = right_border->getPosition();

            // left_border->setPosition(mLeftButtonPos - this->getUITranslation());
            // bottom_border->setPosition(mBottomButtonPos - this->getUITranslation());
            flag = true;
        }
        else if (abs(int(relative_pos.y - bottom_border->getPosition().y)) < 20.0f && 
            relative_pos.x > (this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f)
        )
        {
            Vector2 scale = Vector2{this->getUIViewScale().x, mousePos.y / h};
            this->setUIViewScale(scale);

            bottom_border->setPosition(Vector2{bottom_border->getPosition().x, relative_pos.y});
            left_border->setPosition(Vector2{left_border->getPosition().x, 
                this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y * 0.5f
            });
            left_border->setDimension(Vector2{20.0f, this->getUIBufferViewSize().y});

            // mLeftButtonPos = left_border->getPosition();
            // mBottomButtonPos = bottom_border->getPosition();
            // mRightButtonPos = right_border->getPosition();

            // left_border->setPosition(mLeftButtonPos - this->getUITranslation());
            // bottom_border->setPosition(mBottomButtonPos - this->getUITranslation());
            flag = true;
        }
        else if ((abs(int(relative_pos.x - right_border->getPosition().x)) < 20.0f && 
            relative_pos.y > (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f)) 
            // || mouse_wheel
        )
        {
            // auto pre_y = right_border->getPosition().y + this->getUITranslation().y;
            auto dy = relative_pos.y;

            // SDL_Log("[ResourceMenu] pre dy: %.1f  pos: (%.1f, %.1f)", dy, right_border->getPosition().x, right_border->getPosition().y);

            // if (mouse_wheel)
            // {
            //     dy = right_border->getPosition().y - mouse_wheel * 10.0f;

            //     SDL_Log("[Game] Mouse wheel is scrolling: y: %d", mouse_wheel);
            // }

            if ((dy + h * 0.5f + right_border->getDimension().y * 0.5) > (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y))
            {
                dy = (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y) - h * 0.5f - right_border->getDimension().y * 0.5f;
            }
            else if ((dy + h * 0.5f - right_border->getDimension().y * 0.5f) < (this->getUIBufferPos().y + this->getUIPosOffset().y))
            {
                dy = (this->getUIBufferPos().y + this->getUIPosOffset().y) - h * 0.5f + right_border->getDimension().y * 0.5f;
            }

            right_border->setPosition(Vector2{right_border->getPosition().x, dy});

            // auto cur_y = dy;

            this->setUITranslation(Vector2{this->getUITranslation().x, 
                this->getPosOffsetCoeff() * (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y - (dy + right_border->getDimension().y * 0.5f))
            });

            // left_border->setPosition(mLeftButtonPos - this->getUITranslation());
            // bottom_border->setPosition(mBottomButtonPos - this->getUITranslation());
            // right_border->setPosition(right_border->getPosition() - this->getUITranslation());
            
            // SDL_Log("[ResourceMenu] dy: %.1f pos: (%.1f, %.1f)", dy, right_border->getPosition().x, right_border->getPosition().y);
        }

        this->setUIPosOffset(Vector2{
            w - this->getUIBufferPos().x - this->getUIBufferViewSize().x, 
            h - this->getUIBufferPos().y - this->getUIBufferViewSize().y
        });

        if (flag)
        {
            auto buffer_pos = this->getUIBufferPos() + this->getUIPosOffset();
            auto buffer_size = this->getUIBufferViewSize();
            for (auto node : this->mResourceStack)
            {
                auto elem = (EmptySprite*)node->mNodeValuePointer;
                elem->setPosition(Vector2{buffer_pos.x - (w - buffer_size.x) * 0.5f, elem->getPosition().y});
                elem->setDimension(Vector2{buffer_size.x * 0.9f, buffer_size.y * 0.06f});
            }
        }
    }

    // 更新资源信息
    ResourceManager* resource_manager = this->getGame()->getResourceManager();
    std::queue<TreeNode*> s;
    if (resource_manager)
    {
        TreeNode* root = resource_manager->getResourceTree().findTreeNode("root");
        s.push(root);

        while (!s.empty())
        {
            int size = s.size();
            while (size--)
            {
                TreeNode* node = s.front();
                s.pop();

                if (((ResourceManager::ResourceProperty*)(node->mNodeValuePointer))->mState == ResourceManager::EUnactivited)
                {
                    // std::cout << "resource state:" << ((ResourceManager::ResourceProperty*)(node->mNodeValuePointer))->mState << std::endl;
                    SDL_Log("[ResourceMenu] Add new node START...");

                    Vector2 pos = Vector2{0.0f, 0.0f};
                    std::map<std::string, std::string> root_name;
                    if (!strcmp(node->mNodeName.c_str(), "root"))
                    {
                        auto r = this->mResourceMenuTree.findTreeNode("root");
                        pos = ((EmptySprite*)(r->mNodeValuePointer))->getPosition();
                        pos.y -= ((EmptySprite*)(r->mNodeValuePointer))->getDimension().y;
                        // root_name = ((EmptySprite*)(r->mNodeValuePointer))->getBindTexName();
                    }
                    else
                    {
                        for (auto n : this->mResourceStack)
                        {
                            if (!strcmp(n->mNodeName.c_str(), node->mNodeParent.c_str()) || 
                                !strcmp(n->mNodeParent.c_str(), n->mNodeParent.c_str())
                            )
                            {
                                pos = ((EmptySprite*)(n->mNodeValuePointer))->getPosition();
                                pos.y -= ((EmptySprite*)(n->mNodeValuePointer))->getDimension().y;
                                // root_name = ((EmptySprite*)(n->mNodeValuePointer))->getBindTexName();
                            }
                        }
                    }

                    // pos += Vector2{5.0f, -20.0f};
                    // std::cout << "root_name:" << (*root_name.begin()).second << std::endl;

                    std::map<std::string, std::string> name_map = allocTex(((ResourceManager::ResourceProperty*)(node->mNodeValuePointer))->mType);
                    // Vector2 root_wh = Vector2{0.0f, 0.0f};
                    // Vector2 cur_wh = Vector2{0.0f, 0.0f};

                    // bool flag1 = false, flag2 = false;
                    // for (auto tex : this->getUITextures())
                    // {
                    //     if (!strcmp(tex.first.c_str(), (*name_map.begin()).second.c_str()))
                    //     {
                    //         cur_wh.x = tex.second->getWidth(); 
                    //         cur_wh.y = tex.second->getHeight();
                    //         // std::cout << "[ResourceMenu] Find1..." << std::endl;
                            
                    //         flag1 = true; 
                    //     }

                    //     if (!strcmp(tex.first.c_str(), (*root_name.begin()).second.c_str()))
                    //     {
                    //         root_wh.x = tex.second->getWidth(); 
                    //         root_wh.y = tex.second->getHeight();

                    //         // std::cout << "[ResourceMenu] Find2..." << std::endl;

                    //         flag2 = true;
                    //     }

                    //     if (flag1 && flag2)
                    //     {
                    //         // std::cout << "[ResourceMenu] Find..." << std::endl;
                    //         break;
                    //     }
                    // }   

                    // pos.x = pos.x - (root_wh.x - cur_wh.x) * 0.5;
                    // pos.y = pos.y - root_wh.y;

                    // std::cout << "[ResourceMenu] pos: " << pos.x << ", " << pos.y << std::endl;
                    // std::cout << "[ResourceMenu] W: " << root_wh.x << ",  h: " << root_wh.y << std::endl;

                    std::string text = ((ResourceManager::ResourceProperty*)(node->mNodeValuePointer))->mName;
                    wchar_t pwchar[text.length()] = {'\0'};
                    for (int i = 0; i < (int)text.length(); ++i)
                    {
                        memcpy(&pwchar[i], &text[i], sizeof(char));
                    }
                    std::wstring w_str(pwchar, pwchar + (int)text.length());
                    
                    std::string node_name = node->mNodeName;

                    Button* b = new Button(
                        this,
                        node_name, "button",
                        pos, Vector2{this->getUIBufferViewSize().x * 0.75f, this->getUIBufferViewSize().y * 0.09f}, Vector2{1.0f, 1.0f},
                        Vector3{0.25f, 0.25f, 0.25f}, Vector3{0.75f, 0.75f, 0.75f},
                        w_str, Vector3{0.85f, 0.85f, 0.85f}, 20, Vector2{35.0f, 0.0f}, 
                        name_map, Vector2{-35.0f, 0.0f}, 
                        OpenChildren, [this, b](){
                            // auto resource_manager = this->getGame()->getResourceManager();
                            // auto n = resource_manager->getResourceTree().findTreeNode(node_name);

                            // if (!this->mFlag)
                            // {
                            //     for (auto child : n->mChildren)
                            //     {
                            //         ((ResourceManager::ResourceProperty*)child)->mState = ResourceManager::EDisplay;
                            //     }
                            //     this->mFlag = true;
                            // }
                            // else
                            // {
                            //     for (auto child : n->mChildren)
                            //     {
                            //         ((ResourceManager::ResourceProperty*)child)->mState = ResourceManager::EHidden;
                            //     }
                            //     this->mFlag = false;
                            // }

                            // b->Rename(this->getGame()->getRenderer()->getRenderWindow(), this->getGame()->getRenderer()->getBasicShader(), this->getGame()->getRenderer()->getFontShader());
                        },
                        false
                    );

                    // TextButton* b = this->addButton<TextButton>(
                    //     node_name,
                    //     "button",
                    //     name_map,
                    //     w_str, Vector3{0.75f, 0.75f, 0.75f}, 22,
                    //     pos, OpenChildren, [this, b](){
                    //         // auto resource_manager = this->getGame()->getResourceManager();
                    //         // auto n = resource_manager->getResourceTree().findTreeNode(node_name);

                    //         // if (!this->mFlag)
                    //         // {
                    //         //     for (auto child : n->mChildren)
                    //         //     {
                    //         //         ((ResourceManager::ResourceProperty*)child)->mState = ResourceManager::EDisplay;
                    //         //     }
                    //         //     this->mFlag = true;
                    //         // }
                    //         // else
                    //         // {
                    //         //     for (auto child : n->mChildren)
                    //         //     {
                    //         //         ((ResourceManager::ResourceProperty*)child)->mState = ResourceManager::EHidden;
                    //         //     }
                    //         //     this->mFlag = false;
                    //         // }

                    //         b->Rename(this->getGame()->getRenderer()->getRenderWindow(), this->getGame()->getRenderer()->getBasicShader(), this->getGame()->getRenderer()->getFontShader());
                    //     },
                    //     false, 
                    //     Vector2{1.0f, 1.0f}
                    // );

                    // EmptySprite* e = new EmptySprite{
                    //     this,
                    //     node->mNodeName,
                    //     "default",
                    //     name_map,
                    //     false,
                    //     pos,
                    //     Vector2{1.0f, 1.0f},
                    //     100,
                    //     Vector2{1.0f, 1.0f},
                    //     true,
                    //     w_str,
                    //     Vector3{0.75f, 0.75f, 0.75f},
                    //     22
                    // };

                    TreeNode* stack_node = new TreeNode{
                        node->mNodeName,
                        node->mNodeParent,
                        (EmptySprite*)b
                    };

                    this->mResourceStack.emplace_back(stack_node);
                    ((ResourceManager::ResourceProperty*)(node->mNodeValuePointer))->mState = ResourceManager::EDisplay;
                    flag = true;

                    // auto size = this->getUIBufferViewSize();
                    // float height = 0.f + size.y * 0.50f;
                    // for (auto n : this->mResourceStack)
                    // {
                    //     EmptySprite* e = (EmptySprite*)(n->mNodeValuePointer);
                    //     height += e->getDimension().y;
                    // }
                    
                    // if (height <= size.y)
                    // {
                    //     right_border->setDimension(Vector2{right_border->getDimension().x, size.y});
                    //     this->mPosOffsetCoeff = 0.0f;
                    // }
                    // else
                    // {
                    //     auto h = pow(size.y, 2) / height;

                    //     auto diff = size.y - h;
                    //     if (!Math::NearZero(diff))
                    //     {
                    //         auto coeff = (height - size.y) / diff;

                    //         if (!Math::NearZero(coeff))
                    //         {
                    //             right_border->setDimension(Vector2{right_border->getDimension().x, h});
                    //             this->mPosOffsetCoeff = coeff;
                    //         }
                    //         else
                    //         {
                    //             right_border->setDimension(Vector2{right_border->getDimension().x, size.y});
                    //             this->mPosOffsetCoeff = 0.0f;
                    //         }
                    //     }
                    //     else
                    //     {
                    //         right_border->setDimension(Vector2{right_border->getDimension().x, size.y});
                    //         this->mPosOffsetCoeff = 0.0f;
                    //     }
                    // }

                    // SDL_Log("[ResourceMenu] Pos offset coeff: %.3f", this->mPosOffsetCoeff);
                    SDL_Log("[ResourceMenu] Add new node OVER...");
                }

                for (auto child : node->mChildren)
                {
                    if (child)
                    {
                        s.push(child);
                    }
                }
            }
        }

        // SDL_Log("[ResourceMenu] update over...");
    }

    if (flag)
    {
        auto size = this->getUIBufferViewSize();
        float height = 350.0f;
        for (auto n : this->mResourceStack)
        {
            EmptySprite* e = (EmptySprite*)(n->mNodeValuePointer);
            height += e->getDimension().y;
        }
    
        if (height <= size.y)
        {
            right_border->setDimension(Vector2{right_border->getDimension().x, size.y});
            this->setPosOffsetCoeff(0.0f);
        }
        else
        {
            auto h = pow(size.y, 2) / height;

            auto diff = size.y - h;
            if (!Math::NearZero(diff))
            {
                auto coeff = (height - size.y) / diff;

                if (!Math::NearZero(coeff))
                {
                    right_border->setDimension(Vector2{right_border->getDimension().x, h});
                    this->setPosOffsetCoeff(coeff);
                }
                else
                {
                    right_border->setDimension(Vector2{right_border->getDimension().x, size.y});
                    this->setPosOffsetCoeff(0.0f);
                }
            }
            else
            {
                right_border->setDimension(Vector2{right_border->getDimension().x, size.y});
                this->setPosOffsetCoeff(0.0f);
            }
        }
    }
}

void ResourceMenu::processInput(const uint8_t* keys)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
    mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

    if (!this->getUIElements().empty())
    {
        for (auto elem : this->getUIElements())
        {
            if (!strcmp(elem->getType().c_str(), "button"))
            {
                Button* b = reinterpret_cast<Button*>(elem);
                if (b->containsPoint(mousePos))
                    b->setHighlighted(true);
                else
                    b->setHighlighted(false);
            }
        }
    }

    mousePos = this->mousePosRemapping(mousePos);
    if (!this->mNodeStack.empty())
    {
        for (auto n : this->mNodeStack)
        {
            if (!strcmp((reinterpret_cast<EmptySprite*>(n->mNodeValuePointer))->getType().c_str(), "button"))
            {
                Button* b = reinterpret_cast<Button*>(n->mNodeValuePointer);
                if (b->containsPoint(mousePos))
                    b->setHighlighted(true);
                else
                    b->setHighlighted(false);
            }
        }
    }

    if (!this->mResourceStack.empty())
    {
        for (auto n : this->mResourceStack)
        {
            if (!strcmp((reinterpret_cast<EmptySprite*>(n->mNodeValuePointer))->getType().c_str(), "button") &&
                ((EmptySprite*)n->mNodeValuePointer)->getSpriteState() == EmptySprite::EDisplay
            )
            {
                // SDL_Log("[ResourceMenu] Update resource stack...");
                Button* b = reinterpret_cast<Button*>(n->mNodeValuePointer);
                if (b->containsPoint(mousePos))
                    b->setHighlighted(true);
                else
                    b->setHighlighted(false);
            }
        }
    }

    if (!this->mResourceEventStack.empty())
    {
        // SDL_Log("[ResourceMenu] update resource stack...");
        for (auto n : this->mResourceEventStack)
        {
            if (!strcmp(((EmptySprite*)(n->mNodeValuePointer))->getType().c_str(), "button") &&
                ((EmptySprite*)n->mNodeValuePointer)->getSpriteState() == EmptySprite::EDisplay
            )
            {
                Button* b = reinterpret_cast<Button*>(n->mNodeValuePointer);
                if (b->containsPoint(mousePos))
                    b->setHighlighted(true);
                else
                    b->setHighlighted(false);
            }
        }
    }
}

void ResourceMenu::handleKeyPress(int key)
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
        case (SDL_BUTTON_RIGHT):
        {
            SDL_Log("[ResourceMenu] Left Button is down...");

            if (!this->mResourceStack.empty())
            {
                for (auto n : this->mResourceStack)
                {
                    auto b = (Button*)(n->mNodeValuePointer);
                    if (b->getSpriteState() == EmptySprite::EDisplay &&
                        !strcmp(b->getType().c_str(), "button"))
                    {
                        SDL_Log("[ResourceMenu] Button is display...");
                        if (b && b->getHighlighted())
                        {
                            b->onClick();

                            // bool update = this->mResourceEventTree.updateTreeNode<class Button>("root", b);
                            // if (!update)
                            // {
                            //     SDL_Log("[ResourceMenu] update failed...");
                            // }
                            
                            // this->getGame()->getResourceManager()->addObjectResource(b->getSpriteName());

                            // auto r = this->mResourceEventTree.findTreeNode("root");
                            // if (r)
                            // {
                            //     SDL_Log("[ResourceMenu] children size: %d", (int)r->mChildren.size());
                            //     for (auto n : r->mChildren)
                            //     {
                            //         SDL_Log("[ResourceMenu] resource event button...");

                            //         bool f = false;
                            //         for (auto e : this->mResourceEventStack)
                            //         {
                            //             if (!strcmp(n->mNodeName.c_str(), e->mNodeName.c_str()))
                            //             {
                            //                 f = true;
                            //                 break;
                            //             }
                            //         }

                            //         if (!f)
                            //         {
                            //             ((EmptySprite*)(n->mNodeValuePointer))->setSpriteState(EmptySprite::EDisplay);
                            //             this->mResourceEventStack.emplace_back(n);

                            //             SDL_Log("[ResourceMenu] add resource event button...");
                            //         }
                            //     }
                            // }
                            // else
                            // {
                            //     SDL_Log("[ResourceMenu] root is null...");
                            // }

                            // auto pos = b->getPosition();
                            // auto offset = Vector2{0.0f, 0.0f};
                            // for (auto tex : this->getUITextures())
                            // {
                            //     if (!strcmp(tex.first.c_str(), (*b->getBindTexName().begin()).second.c_str()))
                            //     {
                            //         offset = Vector2{tex.second->getWidth(), tex.second->getHeight()};
                            //         break;
                            //     }
                            // }

                            // for (auto n : this->mResourceEventStack)
                            // {
                            //     auto nb = (Button*)(n->mNodeValuePointer);
                            //     auto p = nb->getPosition();
                            //     auto off = Vector2{0.0f, 0.0f};
                            //     for (auto tex : this->getUITextures())
                            //     {
                            //         if (!strcmp(tex.first.c_str(), (*nb->getBindTexName().begin()).second.c_str()))
                            //         {
                            //             off = Vector2{tex.second->getWidth(), tex.second->getHeight()};
                            //             break;
                            //         }
                            //     }

                            //     nb->setPosition(Vector2{p.x + pos.x - (offset.x + off.x) * 0.5, p.y + pos.y - (offset.y + off.y) * 0.5});
                            // }

                            // break;
                        }
                    }
                }
            }

            break;
        }
        case (SDLK_ESCAPE):
        {
            this->setUIState(EInvisible);
            break;
        }
        case (SDLK_SPACE):
        {
            // 设置为运行态
            this->getGame()->setGameState(Game::EGamePlay);

            this->setUIState(EInvisible);

            // 设置鼠标为相对模式
            this->setRelativeMouseMode(true);
            
            break;
        }
        default:
            break;
        }
}

TreeStruct* ResourceMenu::getResourceMenuTree()
{
    return &this->mResourceMenuTree;
}

void ResourceMenu::nodeAddToStack(TreeNode* node)
{
    this->mNodeStack.emplace_back(node);
}

std::map<std::string, std::string> ResourceMenu::allocTex(const ResourceManager::ResourceType& type)
{
    std::map<std::string, std::string> tex_map;

    switch (type)
    {
        case ResourceManager::ECollection:
        {
            tex_map.emplace("off", "collectionIcon");
            tex_map.emplace("on", "collectionIcon");
            break;
        }
        case ResourceManager::EMeshObject:
        {
            tex_map.emplace("off", "meshIcon");
            tex_map.emplace("on", "meshIcon");
            break;
        }
        case ResourceManager::EImage:
        {
            tex_map.emplace("off", "imageIcon");
            tex_map.emplace("on", "imageIcon");
            break;
        }
        case ResourceManager::ELight:
        {
            tex_map.emplace("off", "lightIcon");
            tex_map.emplace("on", "lightIcon");
            break;
        }
        case ResourceManager::ESkeleton:
        {
            tex_map.emplace("off", "skeletonIcon");
            tex_map.emplace("on", "skeletonIcon");
            break;
        }
        default:
        {
            tex_map.emplace("off", "unknown");
            tex_map.emplace("on", "unknown");
            break;
        }
    }

    return tex_map;
}

void ResourceMenu::bindEvent(const UIBindEvent& event)
{
    UIScreen::bindEvent(event);

    switch (event)
    {
        case OpenChildren:
        {
            


            break;
        }
        default:
            break;
    }


}

TreeStruct* ResourceMenu::getResourceEventTree()
{
    return &(this->mResourceEventTree);
}

void ResourceMenu::setUIBorderPos(const Vector2& left, const Vector2& bottom, const Vector2& right)
{
    this->mLeftButtonPos = left;
    this->mBottomButtonPos = bottom;
    this->mRightButtonPos = right;
}

void ResourceMenu::handleMouseWheel(const int& mouse_wheel)
{

    SDL_Log("[ResourceMenu] Mouse wheel is scrolling...");

    UIScreen::handleMouseWheel(mouse_wheel);

    int x = 0, y = 0;
    Uint32 mouse = SDL_GetMouseState(&x, &y);
    Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    Vector2 relative_pos = Vector2{mousePos.x - (this->getGame()->getRenderer()->getScreenWidth() * 0.5f),
        -mousePos.y + (this->getGame()->getRenderer()->getScreenHeight() * 0.5f)
    };

    float w = this->getGame()->getRenderer()->getScreenWidth();
    float h = this->getGame()->getRenderer()->getScreenHeight();

    if (mouse_wheel && 
        relative_pos.y > (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f) &&
        relative_pos.x > (this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f)
    )
    {
        Button *left_border = nullptr, *bottom_border = nullptr, *right_border = nullptr;
        for (auto elem : this->getUIElements())
        {
            if (!strcmp(elem->getSpriteName().c_str(), "left_border"))
            {   
                left_border = (Button*)(elem);
            }
            else if (!strcmp(elem->getSpriteName().c_str(), "bottom_border"))
            {
                bottom_border = (Button*)(elem);
            }
            else if (!strcmp(elem->getSpriteName().c_str(), "right_border"))
            {
                right_border = (Button*)(elem);
            }
        }
        
        auto dy = right_border->getPosition().y + mouse_wheel * 10.0f;
        SDL_Log("[ResourceMenu] Mouse wheel is scrolling: y: %d %.1f", mouse_wheel, dy);

        if ((dy + h * 0.5f + right_border->getDimension().y * 0.5) > (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y))
        {
            dy = (this->getUIBufferPos().y + this->getUIPosOffset().y + this->getUIBufferViewSize().y) - h * 0.5f - right_border->getDimension().y * 0.5f;
        }
        else if ((dy + h * 0.5f - right_border->getDimension().y * 0.5f) < (this->getUIBufferPos().y + this->getUIPosOffset().y))
        {
            dy = (this->getUIBufferPos().y + this->getUIPosOffset().y) - h * 0.5f + right_border->getDimension().y * 0.5f;
        }

        right_border->setPosition(Vector2{right_border->getPosition().x, dy});

        this->setUITranslation(Vector2{this->getUITranslation().x, 
            this->getPosOffsetCoeff() * (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y - (dy + right_border->getDimension().y * 0.5f))
        });

        // left_border->setPosition(mLeftButtonPos - this->getUITranslation());
        // bottom_border->setPosition(mBottomButtonPos - this->getUITranslation());
        // right_border->setPosition(right_border->getPosition() - this->getUITranslation());
    }
}