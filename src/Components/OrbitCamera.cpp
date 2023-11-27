#include "../../include/Components/OrbitCamera.h"
#include "../../include/Actors/Actor.h"
#include "../../include/General/LevelLoader.h"

OrbitCamera::OrbitCamera(class Actor* owner)
: CameraComponent(owner)
{
    this->mPitchSpeed = 0.0f;
    this->mYawSpeed = 0.0f;
    this->mOffset = Vector3(-400.0f, 0.0f, 0.0f);
    this->mUp = Vector3::UnitZ;
}

OrbitCamera::~OrbitCamera()
{

}

void OrbitCamera::update(float dt)
{
    CameraComponent::update(dt);

    // 更新相机相对于目标的偏移量
    Quaternion yaw = Quaternion(Vector3::UnitZ, this->mYawSpeed * dt);
    this->mOffset = Vector3::Transform(this->mOffset, yaw);
    this->mUp = Vector3::Transform(this->mUp, yaw);

    Vector3 forward = -1.0f * this->mOffset;
    forward.Normalize();
    Vector3 right = Vector3::Cross(this->mUp, forward);
    right.Normalize();

    Quaternion pitch = Quaternion(right, this->mPitchSpeed * dt);
    this->mOffset = Vector3::Transform(this->mOffset, pitch);
    this->mUp = Vector3::Transform(this->mUp, pitch);

    // 计算视图矩阵
    Vector3 target = this->getActor()->getPosition();
    Vector3 cameraPos = target + this->mOffset;
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, this->mUp);
    
    this->setViewMatrix(view);
}

float OrbitCamera::getPitchSpeed() const
{
    return this->mPitchSpeed;
}

float OrbitCamera::getYawSpeed() const
{
    return this->mYawSpeed;
}

Vector3 OrbitCamera::getOffset() const
{
    return this->mOffset;
}

Vector3 OrbitCamera::getUp() const
{
    return this->mUp;
}

void OrbitCamera::setPitchSpeed(float pitchSpeed)
{
    this->mPitchSpeed = pitchSpeed;
}

void OrbitCamera::setYawSpeed(float yawSpeed)
{
    this->mYawSpeed = yawSpeed;
}

void OrbitCamera::setOffset(Vector3 offset)
{
    this->mOffset = offset;
}

void OrbitCamera::setUp(Vector3 up)
{
    this->mUp = up;
}

Component::EComponentType OrbitCamera::getType() const
{
    return EOrbitCamera;
}

void OrbitCamera::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void OrbitCamera::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}