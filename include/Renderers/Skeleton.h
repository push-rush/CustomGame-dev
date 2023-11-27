#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

#include "./General/Cmath.h"

struct BoneTransform
{
    Quaternion mRotation;
    Vector3 mTranslation;

    Matrix4 toMatrix() const;

    // 插值
    static BoneTransform interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};

class Skeleton
{
public:
    Skeleton();
    ~Skeleton();

    struct Bone
    {
        BoneTransform mLocalBindPose;
        std::string mName;
        int mParent;
    };

    bool load(std::string fileName);

    size_t getBonesNum() const;
    const Bone& getBone(size_t idx) const;
    const std::vector<Bone>& getBones() const;
    const std::vector<Matrix4>& getGlobalInvBindPoses() const;

    // 计算骨骼全局反向绑定矩阵
    void computeGlobalInvBindPose();

private:
    // 骨骼
    std::vector<Bone> mBones;

    // 全局绑定位姿
    std::vector<Matrix4> mGlobalInvBindPoses;
};