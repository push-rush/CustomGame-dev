#include "./CameraComponent.h"

struct Spline
{
    std::vector<Vector3> mControlPoints;

    Vector3 compute(size_t startIdx, float t) const
    {
        // 检查起始点索引是否越界
        if (startIdx >= this->mControlPoints.size())
        {
            return this->mControlPoints.back();
        }
        else if (startIdx <= 0)
        {
            return this->mControlPoints.front();
        }
        else if (startIdx + 2 >= this->mControlPoints.size())
        {
            return this->mControlPoints[startIdx];
        }

        // 取出P0~P3
        Vector3 p0 = this->mControlPoints[startIdx - 1];
        Vector3 p1 = this->mControlPoints[startIdx];
        Vector3 p2 = this->mControlPoints[startIdx + 1];
        Vector3 p3 = this->mControlPoints[startIdx + 2];

        // 根据Catmull-Rom方程计算位置
        Vector3 position = 0.5f * 
            ((2 * p1) + (p2 - p0) * t + 
            (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t + 
            (-1.0 * p0 + 3 * p1 - 3 * p2 + p3) * t * t * t    
        );
        return position;
    }   

    size_t getNumPoints() const
    {
        return this->mControlPoints.size();
    }
};

class SplineCamera : public CameraComponent
{
private:
    Spline mSplinePath;
    size_t mIndex;
    float mT;
    float mSpeed;
    bool mPaused;

public:
    SplineCamera(class Actor* owner);
    ~SplineCamera();

    void update(float dt) override;

    // getters
    size_t getIndex() const;
    float getT() const;
    float getSpeed() const;
    bool getPaused() const;
    Spline getSpine() const;

    // setters
    void setIndex(size_t);
    void setT(float);
    void setSpeed(float);
    void setPaused(bool);
    void setSpline(Spline& spline);

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};