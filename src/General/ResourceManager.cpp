#include "../../include/General/ResourceManager.h"
#include "../../include/Renderers/EmptySprite.h"

ResourceManager::ResourceManager(Game* game) :
    mGame(game), mCurSelectMenu("root"), 
    mMeshSeq(0), mImageSeq(0), mSkeletonSeq(0), mDefaultSeq(0)
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::update()
{
    auto r = this->mResourceTree.findTreeNode("root");

    std::queue<TreeNode*> q;
    q.push(r);

    while (!q.empty())
    {
        int size = q.size();
        while (size--)
        {
            auto n = q.front();
            q.pop();
            if (n->mNodeValuePointer)
            {
                auto rep = reinterpret_cast<ResourceManager::ResourceProperty*>(n->mNodeValuePointer);
                if (rep->mState == ResourceManager::EDeleted)
                {
                    this->mResourceTree.deleteTreeNode(n->mNodeName);
                }
            }

            for (auto iter = n->mChildren.begin(); iter != n->mChildren.end(); ++iter)
            {
                if ((*iter))
                {
                    q.push((*iter));
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
    // 创建节点
    TreeNode* node = new TreeNode{
        this->allocDefaultName(rep->mType),
        this->mCurSelectMenu,
        reinterpret_cast<void*>(rep)
    };
    this->mResourceTree.addTreeNode(node);
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
            name = std::string(mSkeletonPrefix) + "." + std::to_string(mSkeletonSeq);
            ++mSkeletonSeq;
            break;
        }
        default:
        {
            name = "default." + std::to_string(mDefaultSeq);
            ++mDefaultSeq;
            break;
        }
    }
}