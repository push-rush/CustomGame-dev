#pragma once

#include <map>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <vector>
#include <istream>
#include <fstream>
#include <queue>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "../Renderers/EmptySprite.h"

struct TreeNode
{
    std::string mNodeName;
    std::string mNodeParent;
    void* mNodeValuePointer;
    std::vector<TreeNode*> mChildren;

    TreeNode(const std::string& name, const std::string& parent, void* t)
    : mNodeName(name), mNodeParent(parent), mNodeValuePointer(t) {}
};

class TreeStruct
{
public:
    TreeStruct();
    ~TreeStruct();

    bool addTreeNode(TreeNode* node);
    
    template<typename T>
    bool updateTreeNode(std::string name, T* value);

    TreeNode* findTreeNode(std::string name);

    bool deleteTree(TreeNode* root);
    bool deleteTreeNode(std::string name);

    int getTreeSize() const;

private:
    TreeNode* mTree;
    int mSize;
};

// template class TreeStruct<EmptySprite>;