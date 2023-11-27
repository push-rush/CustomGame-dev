#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <functional>

#include "./General/Cmath.h"

struct CollisionInfo
{
    // 相交点
    Vector3 mCastPoint;
    // 碰撞面法线
    Vector3 mCollPlaneNormal;
    // 碰撞组件的指针
    class BoxComponent* mCollBoxComp;
    // 碰撞组件的对象指针
    class Actor* mCollActor;
};

class PhysWorld
{
private:
    class Game* mGame;
    std::vector<class BoxComponent*> mBoxComponents;

public:
    PhysWorld(class Game* game);
    ~PhysWorld();

    // 添加/删除碰撞组件
    void addBoxComponent(class BoxComponent* box);
    void removeBoxComponent(class BoxComponent* box);

    // 线段与碰撞组件相交判断
    bool segmentCast(const class LineSegment& segLine, CollisionInfo& outCollInfo);

    // 组件间碰撞判断
    void testPairwise(std::function<void(class Actor*, class Actor*)> f);

    // 扫描和修剪法进行组件间碰撞判断
    void testSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);
};