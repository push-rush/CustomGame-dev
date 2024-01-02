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
        EChosen
    };

    struct ResourceProperty
    {
        ResourceType mType;
        ResourceState mState;
        void* mObject;
    };

    ResourceManager(class Game* game);
    ~ResourceManager();

    void update();
    void addResourceProperty(ResourceProperty* rep);
    std::string allocDefaultName(const ResourceType& type);

private:
    class Game* mGame;
    TreeStruct mResourceTree;
    std::string mCurSelectMenu;

    int mMeshSeq;
    int mImageSeq;
    int mSkeletonSeq;
    int mDefaultSeq;
};
