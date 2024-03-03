#include "../../include/General/ResourceManager.h"

#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/Renderer.h"

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

void ResourceManager::update()
{
    auto acts = this->mGame->getActors();
    auto comps = this->mGame->getRenderer()->getMeshComponents();

    for (auto act : acts)
    {
        if (act->getState() == Actor::EClicked)
        {
            for (auto cp : comps)
            {
                if (cp->getActor() == act)
                {
                    auto node_name = "default." + to_string(cp->getCompID());
                    auto node = this->mResourceTree.findTreeNode(node_name);
                    this->setCurSelectMenu(node->mNodeParent);
                    break;
                }
            }
            break;
        }
    }

    // auto r = this->mResourceTree.findTreeNode("root");

    // std::queue<TreeNode*> q;
    // q.push(r);

    // while (!q.empty())
    // {
    //     int size = q.size();
    //     while (size--)
    //     {
    //         auto n = q.front();
    //         q.pop();
    //         if (n->mNodeValuePointer)
    //         {
    //             auto rep = reinterpret_cast<ResourceManager::ResourceProperty*>(n->mNodeValuePointer);
    //             if (rep->mState == ResourceManager::EDeleted)
    //             {
    //                 this->mResourceTree.deleteTreeNode(n->mNodeName);
    //             }
    //         }

    //         for (auto iter = n->mChildren.begin(); iter != n->mChildren.end(); ++iter)
    //         {
    //             if ((*iter))
    //             {
    //                 q.push((*iter));
    //             }
    //         }
    //     }
    // }

    for (auto rep_name : this->mSelectedResources)
    {
        auto n = this->mResourceTree.findTreeNode(rep_name);
        if (n && n->mNodeValuePointer)
        {
            auto rep = reinterpret_cast<ResourceManager::ResourceProperty*>(n->mNodeValuePointer);

            switch (rep->mState)
            {
                case ResourceManager::EDeleted:
                {
                    this->mResourceTree.deleteTreeNode(n->mNodeName);
                    break;
                }
                case ResourceManager::ERename:
                {
                    

                    break;
                }
                default:
                    break;
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

void ResourceManager::addObjectResource(std::string name)
{
    this->mSelectedResources.emplace_back(name);
}

void ResourceManager::setCurSelectMenu(const std::string& menu)
{
    this->mCurSelectMenu = menu;
}