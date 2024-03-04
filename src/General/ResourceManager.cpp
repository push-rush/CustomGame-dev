#include "../../include/General/ResourceManager.h"

#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/ResourceMenu.h"

#include "../../include/Actors/Actor.h"

#include "../../include/Components/MeshComponent.h"

#include "../../include/Game.h"

ResourceManager::ResourceManager(Game* game) :
    mGame(game), mCurSelectMenu("root"), 
    mMeshSeq(0), mImageSeq(0), mSkeletonSeq(0), mCollectionSeq(0), mLightSeq(0), mDefaultSeq(0)
{
    ResourceProperty* rep = new ResourceProperty{
        nullptr,
        this->allocDefaultName(ResourceManager::ECollection),
        ECollection,
        EUnactivited
    };

    TreeNode* node = new TreeNode{
        "root",
        "-1",
        (void*)(rep)
    };

    this->mResourceTree.addTreeNode(node);
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::update(const bool& cast)
{
    auto acts = this->mGame->getActors();
    auto comps = this->mGame->getRenderer()->getMeshComponents();
    auto resource_menu = this->mGame->getResourceMenu();

    if (cast)
    {
        for (auto cp : comps)
        {
            auto act = cp->getActor();
            if (act && act->getState() == Actor::EClicked)
            {
                auto node_name = "Mesh." + to_string(cp->getCompID());
                auto node = this->mResourceTree.findTreeNode(node_name);
                
                if (node)
                {
                    this->setCurSelectMenu(node->mNodeParent);
                    
                    ResourceProperty* rep = (ResourceProperty*)node->mNodeValuePointer;
                    if (rep)
                    {
                        rep->mState = ResourceManager::EChosen;

                        for (auto iter = this->mPickingResNodes.begin(); iter != this->mPickingResNodes.end();)
                        {
                            auto n = (*iter);
                            if (n && n != node)
                            {
                                ResourceProperty* rp = (ResourceProperty*)n->mNodeValuePointer;
                                
                                if (rp->mState == ResourceManager::EChosen)
                                {
                                    rp->mState = ResourceManager::EDisplay;
                                }
                                else if (rp->mState == ResourceManager::EComp_ChooseFold)
                                {
                                    rp->mState = ResourceManager::EComp_DisplayFold;
                                }
                                else if (rp->mState == ResourceManager::EComp_ChooseUnfold)
                                {
                                    rp->mState = ResourceManager::EComp_DisplayUnfold;
                                }

                                iter = this->mPickingResNodes.erase(iter);
                            }
                            else
                            {
                                ++iter;
                            }
                        }

                        this->addPickingResNode(node);

                        // auto root = this->mResourceTree.findTreeNode("root");
                        // std::queue<TreeNode*> s;
                        // s.push(root);

                        // while (!s.empty())
                        // {
                        //     int size = s.size();
                        //     while (size--)
                        //     {
                        //         TreeNode* node = s.front();
                        //         s.pop();

                        //         if (node)
                        //         {
                        //             ResourceProperty* rp = (ResourceProperty*)node->mNodeValuePointer;
                        //             if (strcmp(node->mNodeName.c_str(), node_name.c_str()))
                        //             {
                        //                 if (rp->mState == ResourceManager::EChosen)
                        //                 {
                        //                     rp->mState = ResourceManager::EDisplay;
                        //                 }
                        //                 else if (rp->mState == ResourceManager::EComp_ChooseFold)
                        //                 {
                        //                     rp->mState = ResourceManager::EComp_DisplayFold;
                        //                 }
                        //                 else if (rp->mState == ResourceManager::EComp_ChooseUnfold)
                        //                 {
                        //                     rp->mState = ResourceManager::EComp_DisplayUnfold;
                        //                 }
                        //             }

                        //             for (auto child : node->mChildren)
                        //             {
                        //                 if (child)
                        //                 {
                        //                     s.push(child);
                        //                 }
                        //             }
                        //         }
                        //     }
                        // }
                    }
                    break;
                }
            }
        }
    }
    else
    {
        if (resource_menu)
        {
            auto selected_node = resource_menu->getCurPickingNode();
            if (selected_node)
            {
                Actor* target = nullptr;
                for (auto cp : comps)
                {
                    auto act = cp->getActor();
                    if (act)
                    {
                        std::string cp_name = "Mesh." + to_string(cp->getCompID());
                        if (!strcmp(selected_node->mNodeName.c_str(), cp_name.c_str()))
                        {
                            act->setState(Actor::EClicked);
                            target = act;

                            // SDL_Log("[ResourceManager] Cur comp name: %s", cp_name.c_str());
                            break;
                        }
                    }
                }

                if (target)
                {
                    for (auto cp : comps)
                    {
                        auto act = cp->getActor();
                        if (act != target)
                        {
                            act->setState(Actor::EActive);
                        }
                    }
                }
            }
        }
    }

    for (auto picking_node : this->mPickingResNodes)
    {
        if (picking_node && picking_node->mNodeValuePointer)
        {
            auto rep = reinterpret_cast<ResourceManager::ResourceProperty*>(picking_node->mNodeValuePointer);

            switch (rep->mState)
            {
                case ResourceManager::EDeleted:
                {
                    this->mResourceTree.deleteTreeNode(picking_node->mNodeName);
                    break;
                }
                case ResourceManager::ERename:
                {

                    break;
                }
                default:
                {
                    SDL_Log("[ResourceManager] Update picking node...");

                    break;
                }
            }
        }
    }
}

/**
 * 添加资源属性信息
*/
void ResourceManager::addResourceProperty(ResourceProperty* rep)
{
    // if (!strcmp(rep->mName.c_str(), "default"))
    // {
    //     rep->mName = rep->mName + std::to_string(this->mDefaultSeq);
    //     this->mDefaultSeq++;
    // }

    if (this->mResourceTree.getTreeSize() < 1000)
    {
        // 创建节点
        TreeNode* node = new TreeNode{
            rep->mName,
            this->mCurSelectMenu,
            (void*)(rep)
        };
        this->mResourceTree.addTreeNode(node);
    }
}

std::string ResourceManager::allocDefaultName(const ResourceType& type)
{
    std::string name;
    switch (type)
    {
        case EMeshObject:
        {
            name = std::string(MeshNamePrefix) + "." + std::to_string(mMeshSeq);
            ++mMeshSeq;
            break;
        }
        case EImage:
        {
            name = std::string(ImageNamePrefix) + "." + std::to_string(mImageSeq);
            ++mImageSeq;
            break;        
        }
        case ESkeleton:
        {
            name = std::string(SkeletonPrefix) + "." + std::to_string(mSkeletonSeq);
            ++mSkeletonSeq;
            break;
        }
        case ECollection:
        {
            name = std::string(CollectionPrefix) + "." + std::to_string(mCollectionSeq);
            ++this->mCollectionSeq;

            break;
        }
        case ELight:
        {
            name = std::string(LightPrefix) + "." + std::to_string(mLightSeq);
            ++this->mLightSeq;
            break;
        }
        default:
        {
            name = "default." + std::to_string(mDefaultSeq);
            ++mDefaultSeq;
            break;
        }
    }

    return name;
}

TreeStruct* ResourceManager::getResourceTree()
{
    return &this->mResourceTree;
}

void ResourceManager::addPickingResNode(TreeNode* res_node)
{
    for (auto n : this->mPickingResNodes)
    {
        if (n == res_node)
        {
            return;
        }
    }
    this->mPickingResNodes.emplace_back(res_node);
}

void ResourceManager::removePickingResNode(TreeNode* res_node)
{
    for (auto iter = this->mPickingResNodes.begin(); iter != this->mPickingResNodes.end(); ++iter)
    {
        if ((*iter) == res_node)
        {
            iter = this->mPickingResNodes.erase(iter);
            break;
        }
    }
}

void ResourceManager::setCurSelectMenu(const std::string& menu)
{
    this->mCurSelectMenu = menu;
}