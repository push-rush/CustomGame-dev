#include "../../include/Components/FollowCamera.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Audios/AudioSystem.h"
#include "../../include/Game.h"
#include "../../include/General/LevelLoader.h"


FollowCamera::FollowCamera(class Actor* owner)
: CameraComponent(owner)
{
    this->mHorizonDist = 300.0f;
    this->mVerticalDist = 120.0f;
    this->mTargetDist = 100.0f;
    this->mSpringConstant = 128.0f;
    this->mActualPos = Vector3::Zero;
    this->mVelocity = Vector3::Zero;
}

FollowCamera::~FollowCamera()
{

}

// getters/setters
float FollowCamera::getHorizonDist() const
{
    return this->mHorizonDist;
}

float FollowCamera::getVerticalDist() const
{
    return this->mVerticalDist;
}   

float FollowCamera::getTargetDist() const
{
    return this->mTargetDist;
}

void FollowCamera::setHorizonDist(float value)
{
    this->mHorizonDist = value;
}

void FollowCamera::setVerticalDist(float value)
{
    this->mVerticalDist = value;
}

void FollowCamera::setTargetDist(float value)
{
    this->mTargetDist = value;
}

Vector3 FollowCamera::computeCameraPos() const
{
    Vector3 cameraPos = this->getActor()->getPosition();
    cameraPos -= this->getActor()->getForward() * this->mHorizonDist;
    cameraPos += this->getActor()->getUp() * this->mVerticalDist;
    return cameraPos;
}

/**
 * 为跟拍相机添加弹性
*/
void FollowCamera::update(float dt)
{
    CameraComponent::update(dt);

    // 计算阻尼
    float dampening = 2.0f * Math::Sqrt(this->mSpringConstant);

    // 计算理想位置
    Vector3 idealPos = this->computeCameraPos();

    // 计算跟拍相机实际位置和理想位置的距离
    Vector3 diff = this->mActualPos - idealPos;
    // 计算弹簧加速度
    Vector3 accel = -this->mSpringConstant * diff - dampening * mVelocity;

    // 更新速度
    this->mVelocity += accel * dt;
    // 更新相机真实位置
    this->mActualPos += this->mVelocity * dt;

    // 计算视图矩阵
    Vector3 target = this->getActor()->getPosition() + this->getActor()->getForward() * this->mTargetDist;
    Matrix4 view = Matrix4::CreateLookAt(this->mActualPos, target, Vector3::UnitZ);
    this->setViewMatrix(view);
}

void FollowCamera::snapToIdeal()
{
    // 初始化时设置实际位置为理想位置
    this->mActualPos = this->computeCameraPos();
    // 相机组件速度初始化为0
    this->mVelocity = Vector3::Zero;

    // 计算目标点和视角
    Vector3 target = this->getActor()->getPosition() + 
        this->getActor()->getForward() * this->mTargetDist;
    Matrix4 view = Matrix4::CreateLookAt(
        this->mActualPos, 
        target, 
        Vector3::UnitZ
    );
    this->setViewMatrix(view);
}

Component::EComponentType FollowCamera::getType() const
{
    return EFollowCamera;
}

void FollowCamera::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void FollowCamera::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}