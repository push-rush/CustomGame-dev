#include "../../include/General/TreeStruct.h"
#include "../../include/Renderers/EmptySprite.h"
#include "../../include/General/ResourceManager.h"

TreeStruct::TreeStruct()
: mTree(nullptr), mSize(0)
{   

}

TreeStruct::~TreeStruct()
{
    if (this->mTree)
    {
        deleteTree(this->mTree);
    }
}

bool TreeStruct::addTreeNode(TreeNode* node)
{
    // bool flag = false;
    // if (!this->mTree)
    // {   // 将指针指向树的头节点
    //     this->mTree = node;
    //     flag = true;
    // }
    // else
    // {
    //     std::queue<TreeNode<TValue>*> q;
    //     q.push(mTree);

    //     while (!q.empty())
    //     {
    //         int size = q.size();
    //         while (size--)
    //         {   
    //             TreeNode<TValue>* n = q.front();
    //             q.pop();

    //             if (!strcmp(n->mNodeName.c_str(), node->mNodeParent.c_str()))
    //             {
    //                 n->mChildren.emplace_back(node);
    //                 flag = true;
    //                 break;
    //             }
    //             else if ((int)n->mChildren.size() > 0)
    //             {
    //                 for (auto child : n->mChildren)
    //                 {
    //                     q.push(child);
    //                 }
    //             }
    //         }

    //         if (flag)
    //         {
    //             break;
    //         }
    //     }
    // }
    
    // return flag;

    if (!this->mTree && !strcmp(node->mNodeName.c_str(), "root"))
    {
        this->mTree = node;
    }
    else
    {
        TreeNode* parent_node = this->findTreeNode(node->mNodeParent);
        if (parent_node)
        {
            parent_node->mChildren.emplace_back(node);
        }
        else
        {
            return false;
        }
    }

    this->mSize += 1;
    return true;
}

bool TreeStruct::deleteTreeNode(std::string name)
{
    if (!strcmp(this->mTree->mNodeName.c_str(), name.c_str()))
    {
        deleteTree(this->mTree);
    }
    else
    {
        TreeNode* node = this->findTreeNode(name);
        TreeNode* parent_node = this->findTreeNode(node->mNodeParent);
        
        if (parent_node && node)
        {
            if ((int)parent_node->mChildren.size() > 0)
            {
                for (auto iter = parent_node->mChildren.begin(); iter != parent_node->mChildren.end(); ++iter)
                {
                    if (!strcmp((*iter)->mNodeName.c_str(), node->mNodeName.c_str()))
                    {
                        iter = parent_node->mChildren.erase(iter);
                        break;
                    }
                }
            }
        }
        else 
        {
            return false;
        }

        // 删除从此节点的子树
        this->deleteTree(node);
    }

    return true;
}

bool TreeStruct::deleteTree(TreeNode* root)
{
    int del_num = 0;

    if (root)
    {
        std::queue<TreeNode*> q;
        q.push(root);

        while (q.empty())
        {
            TreeNode* n = q.front();
            q.pop();

            std::vector<TreeNode*> temp;

            if (n->mChildren.size() > 0)
            {
                for (TreeNode* child : n->mChildren)
                {
                    if (child->mChildren.size() > 0)
                    {
                        q.push(child);
                    }
                    else
                    {
                        temp.emplace_back(child);
                    }
                }
            }
            else
            {
                temp.emplace_back(n);
            }

            del_num += (int)temp.size();
            while (int(temp.size()) > 0)
            {
                TreeNode* p = temp.back();
                temp.erase(temp.end());
                if (p)
                {
                    delete p;
                    p = nullptr;
                }
            }
        }

        if (root)
        {
            delete root;
            root = nullptr;

            del_num += 1;
        }
    }
    else
    {
        return false;
    }

    this->mSize -= del_num;
    return true;
}

template <typename T>
bool TreeStruct::updateTreeNode(std::string name, T* value)
{
    TreeNode* node = this->findTreeNode(name);
    if (node)
    {
        auto temp = node->mNodeValuePointer;
        node->mNodeValuePointer = value;
        if (temp)
        {
            delete temp;
            temp = nullptr;
        }
    }
    else
    {
        return false;
    }
    return true;
}

TreeNode* TreeStruct::findTreeNode(std::string name)
{
    TreeNode* result = nullptr;
    std::queue<TreeNode*> q;
    q.push(mTree);

    while (!q.empty())
    {
        int size = q.size();
        while (size--)
        {
            TreeNode* n = q.front();
            q.pop();

            if (!strcmp(n->mNodeName.c_str(), name.c_str()))
            {
                result = n;
                break;
            }
            else if (n->mChildren.size() > 0)
            {
                for (auto child : n->mChildren)
                {
                    q.push(child);
                }
            }
        }

        if (result)
            break;
    }
    
    return result;
}

int TreeStruct::getTreeSize() const
{
    return this->mSize;
}

// template class TreeStruct<EmptySprite>;
// template class TreeStruct<ResourceManager::ResourceProperty<EmptySprite>>;