/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:38:25
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 20:18:41
 * @FilePath: /CustomGame-dev/include/Renderers/PhysWorld.h
 * @Description: Custom game engine constructing...
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <functional>

#include "./General/Cmath.h"

#include <SDL2/SDL.h>

struct CollisionInfo
{
    // 相交点
    Vector3 mCastPoint;
    // 碰撞面法线
    Vector3 mCollPlaneNormal;
    // 碰撞组件的指针
    class BoxComponent* mCollBoxComp;

    class CircleComponent* mCollCircleComp;

    // 碰撞组件的对象指针
    class Actor* mCollActor;
};

class PhysWorld
{
private:
    class Game* mGame;
    std::vector<class BoxComponent*> mBoxComponents;
    std::vector<class CircleComponent*> mCircleComponents;

public:
    PhysWorld(class Game* game);
    ~PhysWorld();

    // 添加/删除方体碰撞组件
    void addBoxComponent(class BoxComponent* box);
    void removeBoxComponent(class BoxComponent* box);

    // 添加/删除球体碰撞组件
    void addCircleComponent(class CircleComponent* box);
    void removeCircleComponent(class CircleComponent* box);

    // 线段与碰撞组件相交判断
    bool segmentCast(const class LineSegment& segLine, CollisionInfo& outCollInfo);

    // 射线与碰撞组件相交判断
    bool RayCast(const class Ray& ray, CollisionInfo& outCollInfo);

    // 组件间碰撞判断
    void testPairwise(std::function<void(class Actor*, class Actor*)> f);

    // 扫描和修剪法进行组件间碰撞判断
    void testSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

    // getters
    std::vector<BoxComponent*>& getBoxComponents();

    // setters
    
};