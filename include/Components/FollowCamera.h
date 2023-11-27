#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>

#include "./CameraComponent.h"

class FollowCamera : public CameraComponent
{
private:
    // 相机距离主角的水平距离、垂直距离
    float mHorizonDist;
    float mVerticalDist;
    // 目标点距车的水平距离
    float mTargetDist;
    
    // 弹性系数
    float mSpringConstant;
    // 相机实际位置
    class Vector3 mActualPos;
    // 相机跟随速度
    class Vector3 mVelocity;

public:
    FollowCamera(class Actor* owner);
    ~FollowCamera();

    // getters/setters
    float getHorizonDist() const;
    float getVerticalDist() const;
    float getTargetDist() const;
    void setHorizonDist(float value);
    void setVerticalDist(float value);
    void setTargetDist(float value);

    // 计算相机位置
    Vector3 computeCameraPos() const;

    // 更新相机视图矩阵
    void update(float dt) override;

    // 初始化相机组件
    void snapToIdeal();

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};