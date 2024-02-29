#include "../../include/Renderers/ResourceMenu.h"

#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/TextButton.h"
#include "../../include/Renderers/Graphics2d.h"

#include "../../include/General/Cmath.h"
#include "../../include/General/TreeStruct.h"

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
    // SDL_Log("[ResourceMenu] Draw st...");

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

        Texture* menuArrowFold = nullptr;
        Texture* menuArrowUnfold = nullptr;

        bool flag1 = false, flag2 = false;
        for (auto tex : this->getUITextures())
        {   
            if (!strcmp(tex.first.c_str(), "menuArrowFold"))
            {
                menuArrowFold = tex.second;
                flag1 = true;
            }
            else if (!strcmp(tex.first.c_str(), "menuArrowUnfold"))
            {
                menuArrowUnfold = tex.second;
                flag2 = true;
            }

            if (flag1 && flag2)
            {
                break;
            }
        }

        auto resource_tree = this->getGame()->getResourceManager()->getResourceTree();
        for (auto node : this->mResourceStack)
        {
            auto e = (EmptySprite*)(node->mNodeValuePointer);
            if (e->getSpriteState() == EmptySprite::EDisplay || e->getSpriteState() == EmptySprite::ESelected)
            {
                UIScreen::draw(basicShader, spriteShader, fontShader, e);
                
                auto r = resource_tree->findTreeNode(e->getSpriteName());
                auto rp = (ResourceManager::ResourceProperty*)(r->mNodeValuePointer);
                if (rp->mType == ResourceManager::ECollection)
                {
                    auto pos = ((Button*)(e))->getPosition() + this->getUITranslation();
                    auto size = ((Button*)(e))->getDimension();
                    auto tex_off = ((Button*)(e))->getBindTexOffset();
                    
                    spriteShader->setActive();
                    this->getSpriteVerts()->setActive();

                    // SDL_Log("[ResourceMenu] rep state: %d", rp->mState);
                    if (rp->mState == ResourceManager::EComp_ChooseUnfold || 
                        rp->mState == ResourceManager::EComp_DisplayUnfold
                    )
                    {
                        this->drawTexture(spriteShader, menuArrowUnfold, Vector2{pos.x + tex_off.x - 18.0f, pos.y});

                        auto tb = (TextButton*)(e);
                        auto link_pos = tb->getLinkButtonPos() + this->getUITranslation();
                        Renderers::Graphics2d::drawLine(
                            basicShader,
                            Vector2{pos.x + tex_off.x - 20.0f, pos.y - size.y * 0.5f},
                            Vector2{pos.x + tex_off.x - 20.0f, link_pos.y - size.y * 0.5},
                            Color::White,
                            1
                        );
                    }
                    else if (rp->mState == ResourceManager::EComp_ChooseFold ||
                        rp->mState == ResourceManager::EComp_DisplayFold    
                    )
                    {
                        this->drawTexture(spriteShader, menuArrowFold, Vector2{pos.x + tex_off.x - 20.0f, pos.y});
                    }
                }
            }
        }
    }

    auto r = this->mResourceEventTree.findTreeNode("root");
    if (r && r->mNodeValuePointer && (int)(this->mResourceEventStack.size()) > 0)
    {
        for (auto n : this->mResourceEventStack)
        {
            auto b = (Button*)(n->mNodeValuePointer);
            if (b->getSpriteState() == EmptySprite::EDisplay || b->getSpriteState() == EmptySprite::ESelected)
            {
                // SDL_Log("[ResourceMenu] Draw event tree...");
                UIScreen::draw(basicShader, spriteShader, fontShader, b);
            }
        }
    }

    // SDL_Log("[ResourceMenu] Draw over...");
}

void ResourceMenu::update(float dt)
{
    // SDL_Log("[ResourceMenu] Update st...");

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
    Vector2 relative_pos = Vector2{
        mousePos.x - (this->getGame()->getRenderer()->getScreenWidth() * 0.5f),
        -mousePos.y + (this->getGame()->getRenderer()->getScreenHeight() * 0.5f)
    };

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
            this->setUIPosOffset(Vector2{
                w - this->getUIBufferPos().x - this->getUIBufferViewSize().x, 
                h - this->getUIBufferPos().y - this->getUIBufferViewSize().y
            });

            left_border->setPosition(Vector2{relative_pos.x, left_border->getPosition().y});

            bottom_border->setPosition(Vector2{
                this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f + this->getUIBufferViewSize().x * 0.5f, 
                bottom_border->getPosition().y
            });
            bottom_border->setDimension(Vector2{this->getUIBufferViewSize().x, 20.0f});

            flag = true;
        }
        else if (abs(int(relative_pos.y - bottom_border->getPosition().y)) < 20.0f && 
            relative_pos.x > (this->getUIBufferPos().x + this->getUIPosOffset().x - w * 0.5f)
        )
        {
            Vector2 scale = Vector2{this->getUIViewScale().x, mousePos.y / h};
            this->setUIViewScale(scale);
            this->setUIPosOffset(Vector2{
                w - this->getUIBufferPos().x - this->getUIBufferViewSize().x, 
                h - this->getUIBufferPos().y - this->getUIBufferViewSize().y
            });

            bottom_border->setPosition(Vector2{bottom_border->getPosition().x, relative_pos.y});
            left_border->setPosition(Vector2{left_border->getPosition().x, 
                this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f + this->getUIBufferViewSize().y * 0.5f
            });
            left_border->setDimension(Vector2{20.0f, this->getUIBufferViewSize().y});

            flag = true;
        }
        else if ((abs(int(relative_pos.x - right_border->getPosition().x)) < 20.0f && 
            relative_pos.y > (this->getUIBufferPos().y + this->getUIPosOffset().y - h * 0.5f)) 
            // || mouse_wheel
        )
        {
            auto dy = relative_pos.y;

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

            // SDL_Log("[ResourceMenu] dy: %.1f pos: (%.1f, %.1f)", dy, right_border->getPosition().x, right_border->getPosition().y);
        }

        if (flag)
        {
            auto buffer_pos = this->getUIBufferPos() + this->getUIPosOffset();
            auto buffer_size = this->getUIBufferViewSize();
            
            for (auto node : this->mResourceStack)
            {
                auto elem = (EmptySprite*)node->mNodeValuePointer;
                elem->setPosition(Vector2{buffer_pos.x - (w - buffer_size.x) * 0.5f, elem->getPosition().y});
                elem->setDimension(Vector2{buffer_size.x * 1.0f, elem->getDimension().y});
            }

            for (auto node : this->mNodeStack)
            {
                auto elem = (EmptySprite*)node->mNodeValuePointer;
                elem->setPosition(Vector2{buffer_pos.x - (w - buffer_size.x) * 0.5f, elem->getPosition().y});
                // elem->setDimension(Vector2{buffer_size.x * 1.0f, elem->getDimension().y});
            }
            
            SDL_Log("[ResourceMenu] Resource stack size: %d", this->mResourceStack.size());
        }
    }

    // 更新资源信息
    ResourceManager* resource_manager = this->getGame()->getResourceManager();
    std::queue<TreeNode*> s;
    if (resource_manager)
    {
        TreeNode* root = resource_manager->getResourceTree()->findTreeNode("root");
        s.push(root);

        while (!s.empty())
        {
            int size = s.size();
            while (size--)
            {
                TreeNode* node = s.front();
                s.pop();

                auto rp = (ResourceManager::ResourceProperty*)(node->mNodeValuePointer);
                if (rp->mState == ResourceManager::EUnactivited)
                {
                    // auto parent_node = resource_manager->getResourceTree()->findTreeNode(node->mNodeParent);
                    // if (parent_node && parent_node->mNodeValuePointer)
                    // {
                    //     auto parent_rep = (ResourceManager::ResourceProperty*)(parent_node->mNodeValuePointer);

                    //     if (parent_rep->mType == ResourceManager::ECollection && parent_rep->mState == ResourceManager::EFold)
                    //     {
                    //         parent_rep->mState = ResourceManager::EUnfold;
                    //     }
                    // }

                    auto w = this->getGame()->getRenderer()->getScreenWidth();
                    auto h = this->getGame()->getRenderer()->getScreenHeight();
                    auto ui_pos = this->getUIBufferPos() + this->getUIPosOffset() - Vector2{w * 0.5f, h * 0.5f};

                    Vector2 pos = Vector2{0.0f, 0.0f};
                    TextButton* link_tb = nullptr;
                    float text_offset = -this->getUIBufferViewSize().x * 0.5f + 80;
                    float tex_offset = -this->getUIBufferViewSize().x * 0.5f + 30;
                    if (!strcmp(node->mNodeName.c_str(), "root"))
                    {
                        auto r = this->mResourceMenuTree.findTreeNode("root");
                        
                        pos = ((EmptySprite*)(r->mNodeValuePointer))->getPosition();
                        pos.x = ui_pos.x + this->getUIBufferViewSize().x * 0.5f;
                        pos.y -= ((EmptySprite*)(r->mNodeValuePointer))->getDimension().y;
                    }
                    else
                    {
                        TreeNode* parent = nullptr;
                        for (auto n : this->mResourceStack)
                        {
                            if (!strcmp(n->mNodeName.c_str(), node->mNodeParent.c_str()))
                            {
                                parent = n;
                                
                                auto tb = ((TextButton*)(n->mNodeValuePointer));
                                pos = tb->getPosition();
                                // pos.x += tb->getDimension().x * 0.025f;
                                tex_offset = tb->getBindTexOffset().x + tb->getDimension().x * 0.022f;
                                text_offset = tb->getBindTextOffset().x + tb->getDimension().x * 0.022f;

                                pos.y -= tb->getDimension().y;

                                tb->setLinkButtonPos(pos);

                                // link_tb = tb;
                            }
                            else if (!strcmp(n->mNodeParent.c_str(), node->mNodeParent.c_str()))
                            {
                                auto tb = ((TextButton*)(n->mNodeValuePointer));
                                pos = tb->getPosition();
                                pos.y -= tb->getDimension().y;

                                if (parent)
                                {
                                    ((TextButton*)(parent->mNodeValuePointer))->setLinkButtonPos(pos);
                                }

                                link_tb = tb;
                            }
                        }
                    }

                    // pos += Vector2{5.0f, -20.0f};
                    // std::cout << "root_name:" << (*root_name.begin()).second << std::endl;

                    std::map<std::string, std::string> name_map = allocTex(rp->mType);
                    std::string text = rp->mName;
                    wchar_t pwchar[text.length()] = {'\0'};
                    for (int i = 0; i < (int)text.length(); ++i)
                    {
                        memcpy(&pwchar[i], &text[i], sizeof(char));
                    }
                    std::wstring w_str(pwchar, pwchar + (int)text.length());
                    

                    std::string node_name = node->mNodeName;
                    TextButton* b = new TextButton(
                        this,
                        node_name, "button",
                        pos, Vector2{this->getUIBufferViewSize().x * 1.0f, 24}, Vector2{1.0f, 1.0f},
                        Vector3{0.35f, 0.35f, 0.35f}, Vector3{0.35f, 0.35f, 0.35f},
                        w_str, Vector3{0.85f, 0.85f, 0.85f}, 18, Vector2{text_offset, 0.0f}, 
                        name_map, Vector2{tex_offset, 0.0f},
                        OpenChildren, [](){},
                        false
                    );

                    b->setLinkTextButton(link_tb);
                    if (rp->mType == ResourceManager::ECollection)
                    {
                        b->setClickCallback([this, b, node, rp](){
                            SDL_Log("[ResourceMenu] Button is selected...");
                            b->setSpriteState(EmptySprite::ESelected);
                            if (rp->mState == ResourceManager::EComp_DisplayFold || rp->mState == ResourceManager::EComp_ChooseFold)
                            {
                                rp->mState = ResourceManager::EComp_ChooseUnfold;
                            }
                            else if (rp->mState == ResourceManager::EComp_DisplayUnfold || rp->mState == ResourceManager::EComp_ChooseUnfold)
                            {
                                rp->mState = ResourceManager::EComp_ChooseFold;
                            }
                            
                            SDL_Log("rep state: %d", rp->mState);
                        });

                    }
                    else
                    {
                        b->setClickCallback([this, b, rp](){
                            SDL_Log("[ResourceMenu] Button is selected...");
                            b->setSpriteState(EmptySprite::ESelected);
                            rp->mState = ResourceManager::EChosen;
                        });
                    }

                    b->setSpriteState(EmptySprite::EActivited);

                    TreeNode* stack_node = new TreeNode{
                        node->mNodeName,
                        node->mNodeParent,
                        (EmptySprite*)b
                    };

                    this->mResourceStack.emplace_back(stack_node);
                    rp->mState = ResourceManager::EActivited;
                    flag = true;

                    SDL_Log("[ResourceMenu] Add new node name: %s", node_name.c_str());
                }
                else if (rp->mState == ResourceManager::EActivited)
                {
                    // SDL_Log("[ResourceMenu] Activite node...");

                    auto parent = resource_manager->getResourceTree()->findTreeNode(node->mNodeParent);
                    if (parent && parent->mNodeValuePointer)
                    {
                        auto parent_rep = ((ResourceManager::ResourceProperty*)parent->mNodeValuePointer);
                        if (parent_rep->mState == ResourceManager::EComp_HideFold || 
                            parent_rep->mState == ResourceManager::EComp_HideUnfold
                        )
                        {
                            if (rp->mType == ResourceManager::ECollection)
                            {
                                rp->mState = ResourceManager::EComp_HideFold;
                            }
                            else
                            {
                                rp->mState = ResourceManager::EHidden;
                            }
                        }
                        else
                        {
                            if (rp->mType == ResourceManager::ECollection)
                            {
                                rp->mState = ResourceManager::EComp_DisplayFold;
                            }
                            else
                            {
                                rp->mState = ResourceManager::EDisplay;
                            }
                        }
                    }
                    else
                    {
                        if (rp->mType == ResourceManager::ECollection)
                        {
                            rp->mState = ResourceManager::EComp_DisplayFold;
                        }
                        else
                        {
                            rp->mState = ResourceManager::EDisplay;
                        }
                    }
                }
                else if (rp->mState == ResourceManager::EDisplay || 
                    rp->mState == ResourceManager::EComp_DisplayFold || 
                    rp->mState == ResourceManager::EComp_DisplayUnfold
                )
                {
                    // SDL_Log("[ResourceMenu] Display node...");

                    TreeNode* parent = nullptr;
                    TreeNode* sibling = nullptr;
                    for (auto n : this->mResourceStack)
                    {
                        if (!strcmp(n->mNodeName.c_str(), node->mNodeName.c_str()))
                        {
                            // SDL_Log("[ResourceMenu] Text button is display...");

                            auto tb = (TextButton*)(n->mNodeValuePointer);
                            tb->setSpriteState(EmptySprite::EDisplay);
                            
                            TextButton* link_tb = tb->getLinkTextButton();
                            if (link_tb)
                            {
                                auto pos = link_tb->getLinkButtonPos();

                                auto pos_offset = tb->getPosition() - tb->getLinkButtonPos();

                                pos.y -= tb->getDimension().y;
                                tb->setPosition(Vector2{tb->getPosition().x, pos.y});
                                tb->setLinkButtonPos(tb->getPosition() - pos_offset);

                                if (parent)
                                {
                                    auto parent_tb = (TextButton*)(parent->mNodeValuePointer);
                                    parent_tb->setLinkButtonPos(tb->getLinkButtonPos());
                                }
                            }
                            else if (parent)
                            {
                                auto parent_tb = (TextButton*)(parent->mNodeValuePointer);
                                auto pos = parent_tb->getPosition();
                                auto pos_offset = tb->getPosition() - tb->getLinkButtonPos();

                                pos.y -= parent_tb->getDimension().y;
                                tb->setPosition(Vector2{tb->getPosition().x, pos.y});
                                tb->setLinkButtonPos(tb->getPosition() - pos_offset);

                                parent_tb->setLinkButtonPos(tb->getLinkButtonPos());
                            }

                            // if (sibling)
                            // {
                            //     auto tb = (TextButton*)(sibling->mNodeValuePointer);
                            //     auto pos = tb->getLinkButtonPos();
                            //     pos.y -= tb->getDimension().y;
                            //     e->setPosition(Vector2{e->getPosition().x, pos.y});
                            //     // ((TextButton*)(e))->setLinkButtonPos(e->getPosition());

                            //     if (parent)
                            //     {
                            //         ((TextButton*)(parent->mNodeValuePointer))->setLinkButtonPos(pos);
                            //     }
                            // }
                            // else if (parent)
                            // {
                            //     auto tb = (TextButton*)(parent->mNodeValuePointer);
                            //     auto pos = tb->getPosition();
                            //     pos.y -= tb->getDimension().y;
                            //     e->setPosition(Vector2{e->getPosition().x, pos.y});
                            //     // ((TextButton*)(e))->setLinkButtonPos(e->getPosition());

                            //     tb->setLinkButtonPos(pos);
                            // }
                            break;
                        }
                        else if (!strcmp(n->mNodeName.c_str(), node->mNodeParent.c_str()))
                        {
                            // SDL_Log("[ResourceMenu] Find parent text button...");

                            parent = n;
                        }
                        else if (!strcmp(n->mNodeParent.c_str(), node->mNodeParent.c_str()))
                        {
                            // SDL_Log("[ResourceMenu] Find sibling text button...");

                            sibling = n;
                        }
                    }
                }
                else if (rp->mState == ResourceManager::EChosen || 
                    rp->mState == ResourceManager::EComp_ChooseFold || 
                    rp->mState == ResourceManager::EComp_ChooseUnfold
                )
                {
                    // SDL_Log("[ResourceMenu] Choose node...");

                    for (auto n : this->mResourceStack)
                    {
                        TreeNode* parent = nullptr;
                        TreeNode* sibling = nullptr;
                        if (!strcmp(n->mNodeName.c_str(), node->mNodeName.c_str()))
                        {
                            auto tb = (TextButton*)(n->mNodeValuePointer);
                            tb->setSpriteState(EmptySprite::ESelected);

                            TextButton* link_tb = tb->getLinkTextButton();
                            if (link_tb)
                            {
                                auto pos = link_tb->getLinkButtonPos();

                                auto pos_offset = tb->getPosition() - tb->getLinkButtonPos();

                                pos.y -= tb->getDimension().y;
                                tb->setPosition(Vector2{tb->getPosition().x, pos.y});
                                tb->setLinkButtonPos(tb->getPosition() - pos_offset);

                                if (parent)
                                {
                                    auto parent_tb = (TextButton*)(parent->mNodeValuePointer);
                                    parent_tb->setLinkButtonPos(tb->getLinkButtonPos());
                                }
                            }
                            else if (parent)
                            {
                                auto parent_tb = (TextButton*)(parent->mNodeValuePointer);
                                auto pos = parent_tb->getPosition();
                                auto pos_offset = tb->getPosition() - tb->getLinkButtonPos();

                                pos.y -= parent_tb->getDimension().y;
                                tb->setPosition(Vector2{tb->getPosition().x, pos.y});
                                tb->setLinkButtonPos(tb->getPosition() - pos_offset);

                                parent_tb->setLinkButtonPos(tb->getLinkButtonPos());
                            }

                            break;
                        }
                        else if (!strcmp(n->mNodeName.c_str(), node->mNodeParent.c_str()))
                        {
                            // SDL_Log("[ResourceMenu] Find parent text button...");

                            parent = n;
                        }
                        else if (!strcmp(n->mNodeParent.c_str(), node->mNodeParent.c_str()))
                        {
                            // SDL_Log("[ResourceMenu] Find sibling text button...");

                            sibling = n;
                        }
                    }
                }
                else if (rp->mState == ResourceManager::EHidden ||
                    rp->mState == ResourceManager::EComp_HideFold ||
                    rp->mState == ResourceManager::EComp_HideUnfold
                )
                {
                    // SDL_Log("[ResourceMenu] Hide node...");

                    TreeNode* parent = nullptr;
                    for (auto n : this->mResourceStack)
                    {
                        if (!strcmp(n->mNodeName.c_str(), node->mNodeParent.c_str()))
                        {
                            parent = n;
                        }
                        else if (!strcmp(n->mNodeName.c_str(), node->mNodeName.c_str()))
                        {
                            auto e = (EmptySprite*)(n->mNodeValuePointer);
                            e->setSpriteState(EmptySprite::EUninvisible);
                            
                            if (parent)
                            {
                                auto tb = ((TextButton*)(parent->mNodeValuePointer));
                                tb->setLinkButtonPos(tb->getPosition());
                            }
                            break;
                        }
                    }
                }
                
                if (rp->mState == ResourceManager::EFold || rp->mState == ResourceManager::EComp_ChooseFold || 
                    rp->mState == ResourceManager::EComp_DisplayFold || rp->mState == ResourceManager::EComp_HideFold || 
                    rp->mState == ResourceManager::EComp_HideUnfold
                )
                {
                    // SDL_Log("[ResourceMenu] Fold node...");

                    // rp->mState == ResourceManager::EChosen;
                    for (auto child : node->mChildren)
                    {
                        auto temp_rep = ((ResourceManager::ResourceProperty*)(child->mNodeValuePointer));
                        if (temp_rep)
                        {
                            // SDL_Log("[ResourceMenu] fold child: state=%d", temp_rep->mState);
                            if (temp_rep->mType == ResourceManager::ECollection)
                            {
                                if (temp_rep->mState == ResourceManager::EComp_ChooseFold ||
                                    temp_rep->mState == ResourceManager::EComp_DisplayFold
                                )
                                {
                                    temp_rep->mState = ResourceManager::EComp_HideFold;
                                }
                                else if (temp_rep->mState == ResourceManager::EComp_ChooseUnfold ||
                                    temp_rep->mState == ResourceManager::EComp_DisplayUnfold
                                )
                                {
                                    temp_rep->mState = ResourceManager::EComp_HideUnfold;
                                }
                                else
                                {
                                    // SDL_Log("[ResourceMenu] fold child: state=%d", temp_rep->mState);
                                }
                            }
                            else
                            {
                                temp_rep->mState = ResourceManager::EHidden;
                            }
                        }
                    }
                }
                else if (rp->mState == ResourceManager::EUnfold || 
                    rp->mState == ResourceManager::EComp_ChooseUnfold || 
                    rp->mState == ResourceManager::EComp_DisplayUnfold
                )
                {
                    // SDL_Log("[ResourceMenu] Unfold node...");

                    // rp->mState == ResourceManager::EChosen;
                    for (auto child : node->mChildren)
                    {
                        auto temp_rep = ((ResourceManager::ResourceProperty*)(child->mNodeValuePointer));

                        if (temp_rep)
                        {
                            if (temp_rep->mType == ResourceManager::ECollection)
                            {
                                if (temp_rep->mState == ResourceManager::EComp_ChooseFold ||
                                    temp_rep->mState == ResourceManager::EComp_HideFold
                                )
                                {
                                    temp_rep->mState = ResourceManager::EComp_DisplayFold;
                                }
                                else if (temp_rep->mState == ResourceManager::EComp_ChooseUnfold ||
                                    temp_rep->mState == ResourceManager::EComp_HideUnfold
                                )
                                {
                                    temp_rep->mState = ResourceManager::EComp_DisplayUnfold;
                                }
                            }
                            else
                            {
                                temp_rep->mState = ResourceManager::EDisplay;
                            }
                        }
                    }
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

    // SDL_Log("[ResourceMenu] Update over...");
}

void ResourceMenu::processInput(const uint8_t* keys)
{
    // SDL_Log("[ResourceMenu] Process input st...");

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
            auto e = (EmptySprite*)n->mNodeValuePointer;
            if (!strcmp(e->getType().c_str(), "button") &&
                (e->getSpriteState() == EmptySprite::EDisplay ||
                e->getSpriteState() == EmptySprite::ESelected) 
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
        for (auto n : this->mResourceEventStack)
        {
            auto e = (EmptySprite*)n->mNodeValuePointer;

            if (!strcmp(e->getType().c_str(), "button") &&
                (e->getSpriteState() == EmptySprite::EDisplay ||
                e->getSpriteState() == EmptySprite::ESelected)
            )
            {
                // SDL_Log("[ResourceMenu] update resource event stack...");

                Button* b = reinterpret_cast<Button*>(n->mNodeValuePointer);
                if (b->containsPoint(mousePos))
                    b->setHighlighted(true);
                else
                    b->setHighlighted(false);
            }
        }
    }

    // SDL_Log("[ResourceMenu] Process input over...");
}

void ResourceMenu::handleKeyPress(int key)
{
    // SDL_Log("[ResourceMenu] Handle key press over...");

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

            if (!this->mResourceStack.empty())
            {
                bool flag = false;
                Button* tb = nullptr;
                auto resource_manager = this->getGame()->getResourceManager();

                if (resource_manager)
                {
                    for (auto n : this->mResourceStack)
                    {
                        auto b = (Button*)(n->mNodeValuePointer);
                        if (b && b->getHighlighted())
                        {
                            b->onClick();
                            flag = true;

                            auto resource_tree = resource_manager->getResourceTree();
                            auto tree_node = resource_tree->findTreeNode(n->mNodeName);
                            if (tree_node && tree_node->mNodeValuePointer)
                            {
                                auto temp_rep = (ResourceManager::ResourceProperty*)(tree_node->mNodeValuePointer);
                                if (temp_rep->mType == ResourceManager::ECollection)
                                {
                                    resource_manager->setCurSelectMenu(tree_node->mNodeName);
                                }
                            }
                        }
                        else if (b->getSpriteState() == EmptySprite::ESelected)
                        {
                            tb = b;
                        }
                    }

                    if (flag && tb)
                    {
                        auto resource_tree = resource_manager->getResourceTree();
                        auto node = resource_tree->findTreeNode(tb->getSpriteName());
                        if (node && node->mNodeValuePointer)
                        {
                            auto rep = ((ResourceManager::ResourceProperty*)node->mNodeValuePointer);

                            if (rep->mState == ResourceManager::EHidden ||
                                rep->mState == ResourceManager::EComp_HideFold ||
                                rep->mState == ResourceManager::EComp_HideUnfold
                            )
                            {
                                tb->setSpriteState(EmptySprite::EUninvisible);
                            }
                            else
                            {
                                tb->setSpriteState(EmptySprite::EDisplay);

                                if (rep->mType == ResourceManager::ECollection)
                                {
                                    if (rep->mState == ResourceManager::EComp_ChooseFold)
                                    {
                                        rep->mState = ResourceManager::EComp_DisplayFold;
                                    }
                                    else if (rep->mState == ResourceManager::EComp_ChooseUnfold)
                                    {
                                        rep->mState = ResourceManager::EComp_DisplayUnfold;
                                    }
                                }
                                else
                                {
                                    rep->mState = ResourceManager::EDisplay;
                                }
                            }
                        }
                    }
                }
            }

            if (!this->mResourceEventStack.empty())
            {
                // auto r = this->mResourceEventTree.findTreeNode("root");
                // if (r && r->mNodeValuePointer)
                // {
                //     auto b = (Button*)r->mNodeValuePointer;
                //     b->setSpriteState(EmptySprite::EDisplay);
                //     SDL_Log("[ResourceMenu] Sprite name: %s", b->getSpriteName().c_str());

                //     r->mNodeValuePointer = nullptr;
                // }

                for (auto n : this->mResourceEventStack)
                {
                    auto e = (EmptySprite*)(n->mNodeValuePointer);
                    
                    if ((e->getSpriteState() == EmptySprite::EDisplay || e->getSpriteState() == EmptySprite::ESelected) && 
                        !strcmp(e->getType().c_str(), "button")
                    )
                    {
                        auto b = (Button*)e;
                        if (b && b->getHighlighted())
                        {
                            b->onClick();
                        }
                    }
                    e->setSpriteState(EmptySprite::EUninvisible);    
                }
            }
            break;
        }
        case (SDL_BUTTON_RIGHT):
        {
            SDL_Log("[ResourceMenu] Right Button is down...");

            auto root = this->mResourceEventTree.findTreeNode("root");
            if (root && root->mNodeValuePointer)
            {
                auto b = (Button*)root->mNodeValuePointer;
                b->setSpriteState(EmptySprite::EDisplay);
                SDL_Log("[ResourceMenu] Sprite name: %s", b->getSpriteName().c_str());

                root->mNodeValuePointer = nullptr;
            }

            if (!this->mResourceStack.empty())
            {
                bool flag = false;
                Button *tb = nullptr;
                auto resource_manager = this->getGame()->getResourceManager();
                auto resource_tree = resource_manager->getResourceTree();
                for (auto n : this->mResourceStack)
                {
                    auto b = (Button*)(n->mNodeValuePointer);
                    if ((b->getSpriteState() == EmptySprite::EDisplay || b->getSpriteState() == EmptySprite::ESelected) && b->getHighlighted())
                    {
                        SDL_Log("[ResourceMenu] Button is display...");

                        b->setSpriteState(EmptySprite::ESelected);
                        root->mNodeValuePointer = (void*)b;
                        
                        if (resource_manager)
                        {
                            auto temp_node = resource_tree->findTreeNode(n->mNodeName);
                            auto temp_rep = (ResourceManager::ResourceProperty*)(temp_node->mNodeValuePointer);

                            if (temp_rep)
                            {
                                if (temp_rep->mType == ResourceManager::ECollection)
                                {
                                    resource_manager->setCurSelectMenu(temp_node->mNodeName);

                                    if (temp_rep->mState == ResourceManager::EComp_DisplayFold ||
                                        temp_rep->mState == ResourceManager::EComp_HideFold
                                    )
                                    {
                                        temp_rep->mState = ResourceManager::EComp_ChooseFold;
                                    }
                                    else if (temp_rep->mState == ResourceManager::EComp_DisplayUnfold ||
                                        temp_rep->mState == ResourceManager::EComp_HideUnfold
                                    )
                                    {
                                        temp_rep->mState = ResourceManager::EComp_ChooseUnfold;
                                    }
                                }
                                else
                                {
                                    temp_rep->mState = ResourceManager::EChosen;
                                }
                            }
                            // auto tree_node = ((ResourceManager::ResourceProperty*)(resource_tree->findTreeNode(b->getSpriteName())->mNodeValuePointer));
                            // if (tree_node && tree_node->mType == ResourceManager::ECollection)
                            // {
                            //         tree_node->mState = ResourceManager::EUnfold;
                            // }
                        }

                        auto pos = b->getPosition();
                        auto size = b->getDimension();
                        
                        int x = 0, y = 0;
                        Uint32 mouse = SDL_GetMouseState(&x, &y);
                        Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
                        Vector2 relative_pos = Vector2{mousePos.x - (this->getGame()->getRenderer()->getScreenWidth() * 0.5f),
                            -mousePos.y + (this->getGame()->getRenderer()->getScreenHeight() * 0.5f)
                        };

                        auto next_dy = this->getUITranslation().y;
                        EmptySprite* bg = nullptr;
                        for (auto child : root->mChildren)
                        {
                            auto e = ((EmptySprite*)child->mNodeValuePointer);
                            e->setSpriteState(EmptySprite::EDisplay);

                            if (bg)
                            {
                                next_dy += e->getDimension().y;
                                e->setPosition(Vector2{relative_pos.x - size.x * 0.5f, relative_pos.y - next_dy + bg->getDimension().y * 0.5f});
                            }
                            else if (!strcmp(e->getType().c_str(), "default"))
                            {
                                e->setPosition(Vector2{relative_pos.x - size.x * 0.5f, relative_pos.y - next_dy});
                                bg = e;

                                for (auto tex : this->getUITextures())
                                {
                                    if (!strcmp((*bg->getBindTexName().begin()).second.c_str(), tex.first.c_str()))
                                    {
                                        bg->setDimension(Vector2{tex.second->getWidth(), tex.second->getHeight()});
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                next_dy += e->getDimension().y;
                                e->setPosition(Vector2{relative_pos.x - size.x * 0.5f, relative_pos.y - next_dy});
                            }
                        }
                        
                        // break;
                        flag = true;
                    }
                    else if (b->getSpriteState() == EmptySprite::ESelected)
                    {
                        tb = b;
                    }
                }

                if (flag && tb)
                {
                    tb->setSpriteState(EmptySprite::EDisplay);
                    
                    auto tb_node = resource_tree->findTreeNode(tb->getSpriteName());
                    auto tb_rep = (ResourceManager::ResourceProperty*)(tb_node->mNodeValuePointer);

                    if (tb_rep)
                    {
                        if (tb_rep->mType == ResourceManager::ECollection)
                        {
                            if (tb_rep->mState == ResourceManager::EComp_ChooseFold ||
                                tb_rep->mState == ResourceManager::EComp_HideFold
                            )
                            {
                                tb_rep->mState = ResourceManager::EComp_DisplayFold;
                            }
                            else if (tb_rep->mState == ResourceManager::EComp_ChooseUnfold ||
                                tb_rep->mState == ResourceManager::EComp_HideUnfold
                            )
                            {
                                tb_rep->mState = ResourceManager::EComp_DisplayUnfold;
                            }
                        }
                        else
                        {
                            tb_rep->mState = ResourceManager::EDisplay;
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

    // SDL_Log("[ResourceMenu] Handle key press over...");
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

void ResourceMenu::bindEvent(const UIBindEvent& event_id, Button* b)
{
    UIScreen::bindEvent(event_id, b);

    switch (event_id)
    {
        case OpenChildren:
        {
            break;
        }
        case UIRename:
        {
            SDL_Log("[ResourceMenu] Rename text button name...");

            auto r = this->mResourceEventTree.findTreeNode("root");
            if (r && r->mNodeValuePointer)
            {
                TextButton* tb = (TextButton*)(r->mNodeValuePointer);
                auto renderer = this->getGame()->getRenderer();
                tb->Rename(renderer->getRenderWindow(), renderer->getBasicShader(), renderer->getSpriteShader(), renderer->getFontShader());
            }

            break;
        }
        case UICreateCollection:
        {
            SDL_Log("[ResourceMenu] New Collection...");

            ResourceManager::ResourceProperty* rep = new ResourceManager::ResourceProperty{
                "default",
                ResourceManager::ECollection,
                ResourceManager::EUnactivited
            };
            this->getGame()->getResourceManager()->addResourceProperty(rep);

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

void ResourceMenu::updateResourceEventStack()
{
    auto root = this->mResourceEventTree.findTreeNode("root");
    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty())
    {
        int size = q.size();
        while (size--)
        {
            auto n = q.front();
            q.pop();

            for (auto child : n->mChildren)
            {
                if (child->mNodeValuePointer)
                {
                    this->mResourceEventStack.emplace_back(child);
                }
                q.push(child);
            }
        }   
    }
}