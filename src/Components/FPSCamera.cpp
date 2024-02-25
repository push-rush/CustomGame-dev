#include "../../include/Actors/Actor.h"
#include "../../include/Components/FPSCamera.h"
#include "../../include/General/LevelLoader.h"

#include "SDL.h"

FPSCamera::FPSCamera(class Actor* owner)
: CameraComponent(owner)
{
    this->mMaxPitch = Math::Pi * 0.33f;
    this->mPitch = 0.0f;
    this->mPitchSpeed = 0.0f;
}

FPSCamera::~FPSCamera()
{

}

void FPSCamera::update(float dt)
{
    // 更新父类组件
    CameraComponent::update(dt);
    Vector3 cameraPos = this->getActor()->getPosition() + Vector3{0.0f, -250.0f, 100.0f};

    // 更新相机位置
    // 更新相机俯仰角度
    this->mPitch += this->mPitchSpeed * dt;
    // 限制俯仰角度
    this->mPitch = Math::Clamp(this->mPitch, -this->mMaxPitch, this->mMaxPitch);
    // 得到相机相对于角色的俯仰
    Quaternion quat = Quaternion(this->getActor()->getRight(), this->mPitch);

    // 旋转角色的前向向量
    Vector3 viewForward = Vector3::Transform(this->getActor()->getForward(), quat);

    // 计算得到视角前向位置前100单元的位置
    Vector3 target = cameraPos + viewForward * 100.0f;

    // 得到相机视角下的向上向量
    Vector3 up = Vector3::Transform(Vector3::UnitZ, quat);
    
    // 计算相机视图转换矩阵
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    this->setViewMatrix(view); 

    // SDL_Log("Update pitch: %f", mPitch);
    // SDL_Log("forward: [%f %f %f]", this->getActor()->getForward().x, this->getActor()->getForward().y, this->getActor()->getForward().z);
}

void FPSCamera::setPitchSpeed(float pitchSpeed)
{
    this->mPitchSpeed = pitchSpeed;
}

void FPSCamera::setPitch(float pitch)
{
    this->mPitch = pitch;
}

void FPSCamera::setMaxPitch(float maxPitch)
{
    this->mMaxPitch = maxPitch;
}

float FPSCamera::getPitchSpeed() const
{
    return this->mPitchSpeed;
}

float FPSCamera::getMaxPitch() const
{
    return this->mMaxPitch;
}

float FPSCamera::getPitch() const
{   
    return this->mPitch;
}

Component::EComponentType FPSCamera::getType() const
{
    return EFPSCamera;
}

void FPSCamera::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void FPSCamera::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}