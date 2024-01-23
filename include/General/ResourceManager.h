#pragma once

#include <string>

#include "./TreeStruct.h"

#define MeshNamePrefix "Mesh"
#define ImageNamePrefix "Image"
#define mSkeletonPrefix "Skeleton"

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
        ECamera
    };

    enum ResourceState 
    {
        EHidden,
        EDisplay,
        EUnSelected,
        EDeleted,
        EChosen,
        EUnactivited,
        ERename
    };

    struct ResourceProperty
    {
        std::string mName;
        ResourceType mType;
        ResourceState mState;
    };

    ResourceManager(class Game* game);
    ~ResourceManager();

    void update();
    void addResourceProperty(ResourceProperty* rep);
    std::string allocDefaultName(const ResourceType& type);

    void addObjectResource(std::string name);

    // getters
    TreeStruct& getResourceTree();

private:
    class Game* mGame;
    TreeStruct mResourceTree;
    std::string mCurSelectMenu;

    std::vector<std::string> mSelectedResources;

    int mMeshSeq;
    int mImageSeq;
    int mSkeletonSeq;
    int mDefaultSeq;
};
