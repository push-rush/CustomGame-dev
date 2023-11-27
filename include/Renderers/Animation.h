#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "./General/Cmath.h"

class Animation
{
public:
    Animation(/* args */);
    ~Animation();

    bool load(const std::string& fileName);
    size_t getNumBones() const;
    size_t getNumFrames() const;
    float getDuration() const;
    float getFrameDuration() const;

    void getGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;

private:
    size_t mNumBones;
    size_t mNumFrames;
    float mDuration;
    float mFrameDuration;

    std::vector<std::vector<class BoneTransform>> mTracks;
};