#pragma once

#include <string>

#include "./TreeStruct.h"

#define MeshNamePrefix "Mesh"
#define ImageNamePrefix "Image"
#define SkeletonPrefix "Skeleton"
#define CollectionPrefix "Collection"
#define LightPrefix "Light"

class ResourceManager
{   
public:
    enum ResourceType
    {
        ECollection,
        EMeshObject,
        ESkeleton,
        EImage,
        ELight,
        ECamera,
    };

    enum ResourceState 
    {
        EDeleted,
        ERename,
        EUnSelected,
        EHidden,
        EActivited,
        EDisplay,
        EUnactivited,
        EChosen,
        EFold,
        EUnfold,
        EComp_HideFold = EHidden + EFold,
        EComp_HideUnfold = EHidden + EUnfold,
        EComp_DisplayFold = EDisplay + EFold,
        EComp_DisplayUnfold = EDisplay + EUnfold,
        EComp_ChooseFold = EChosen + EFold,
        EComp_ChooseUnfold = EChosen + EUnfold,
    };

    struct ResourceProperty
    {
        class Component* mBindComp;
        std::string mName;
        ResourceType mType;
        ResourceState mState;
    };

    ResourceManager(class Game* game);
    ~ResourceManager();

    void update(const bool& cast);
    void addResourceProperty(ResourceProperty* rep);
    std::string allocDefaultName(const ResourceType& type);

    void removePickingResNode(TreeNode* res_node);

    void addPickingResNode(TreeNode* res_node);

    // setters
    void setCurSelectMenu(const std::string& menu);

    // getters
    TreeStruct* getResourceTree();

private:
    class Game* mGame;
    TreeStruct mResourceTree;
    std::string mCurSelectMenu;

    std::vector<TreeNode*> mPickingResNodes;

    int mMeshSeq;
    int mImageSeq;
    int mSkeletonSeq;
    int mCollectionSeq;
    int mLightSeq;
    int mDefaultSeq;
};
