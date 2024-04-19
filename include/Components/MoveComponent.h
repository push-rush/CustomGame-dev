/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:38:25
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 18:51:11
 * @FilePath: /CustomGame-dev/include/Components/MoveComponent.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "./Component.h"

#include <SDL2/SDL.h>

class MoveComponent : public Component
{
public:
    MoveComponent();
    MoveComponent(class Actor* owner, int updateOrder = 10);
    ~MoveComponent();

    void update(float dt) override;

    float getAngularSpeed() const;
    float getForwardSpeed() const;
    void setAngularSpeed(float angularSpeed);
    void setForwardSpeed(float forwardSpeed);
    void setStrafeSpeed(float strafeSpeed);
    void setPitchSpeed(float pitchSpeed);

    EComponentType getType() const override;

    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;

private:
    float mAngularSpeed; // 旋转速度
    float mForwardSpeed; // 前进速度
    float mStrafeSpeed;  // 侧行速度
    float mPitchSpeed;   // 俯仰速度（不需要）
};