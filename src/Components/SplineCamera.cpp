#include "../../include/Components/SplineCamera.h"
#include "../../include/Actors/Actor.h"
#include "../../include/General/LevelLoader.h"

SplineCamera::SplineCamera(class Actor* owner)
: CameraComponent(owner)
{
    this->mIndex = 1;
    this->mSpeed = 0.5f;
    this->mT = 0.0f;
    this->mPaused = true;
}

SplineCamera::~SplineCamera()
{

}

void SplineCamera::update(float dt)
{
    CameraComponent::update(dt);

    if (!this->mPaused)
    {
        this->mT += this->mSpeed * dt;
        if (this->mT >= 1.0)
        {
            if (this->mIndex < this->mSplinePath.getNumPoints() - 3)
            {
                ++mIndex;
                this->mT -= 1.0f;
            }
            else
            {
                this->mPaused = true;
            }
        }

        // 根据样条曲线计算相机当前位置
        Vector3 cameraPos = this->mSplinePath.compute(this->mIndex, this->mT);
        // 目标点为当前mT的小增量后的点
        Vector3 target = this->mSplinePath.compute(this->mIndex, this->mT + 0.01f);

        // 假定相机移动过程中不发生翻转
        const Vector3 up = Vector3::UnitZ;
        Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
        this->setViewMatrix(view);
    }
}

size_t SplineCamera::getIndex() const
{
    return this->mIndex;
}

float SplineCamera::getT() const
{
    return this->mT;
}

float SplineCamera::getSpeed() const
{
    return this->mSpeed;
}

bool SplineCamera::getPaused() const
{
    return this->mPaused;
}

Spline SplineCamera::getSpine() const
{
    return this->mSplinePath;
}

// setters
void SplineCamera::setIndex(size_t index)
{
    this->mIndex = index;
}

void SplineCamera::setT(float t)
{
    this->mT = t;
}

void SplineCamera::setSpeed(float speed)
{
    this->mSpeed = speed;
}

void SplineCamera::setPaused(bool paused)
{
    this->mPaused = paused;
}

void SplineCamera::setSpline(Spline& spline)
{
    this->mSplinePath = spline;
}

Component::EComponentType SplineCamera::getType() const
{
    return ESplineCamera;
}

void SplineCamera::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void SplineCamera::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}