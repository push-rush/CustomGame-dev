#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <array>
#include <algorithm>

#include "./General/Cmath.h"

/**
 * 射线
*/
struct Ray
{
    Vector3 origin;
    Vector3 direction;

    Ray(const Vector3& _origin, const Vector3& _direction) : origin(_origin), direction(_direction) {}
};


/**
 * 线段
*/
struct LineSegment
{
    Vector3 mStart;
    Vector3 mEnd;

    LineSegment(const Vector3& start, const Vector3& end)
    {
        this->mStart = start;
        this->mEnd = end;
    }

    Vector3 calPointOnSegment(float t) const
    {   // 0 <= t <= 1
        return this->mStart + (this->mEnd - this->mStart) * t;
    }

    float minDistSq(const Vector3& point) const
    {
        Vector3 ab = this->mEnd - this->mStart;
        Vector3 ba = -1.0 * ab;

        Vector3 ac = point - this->mStart;
        Vector3 bc = point - this->mEnd;

        float minDistSq = 0.0f;
        float leftDot = Vector3::Dot(ab, ac);
        float rightDot = Vector3::Dot(ba, bc);
        if (leftDot < 0.0f && rightDot > 0.0f)
        {
            minDistSq = ac.LengthSq();
        }
        else if (leftDot > 0.0f && rightDot < 0.0f)
        {
            minDistSq = bc.LengthSq();
        }
        else
        {
            minDistSq = ab.LengthSq() * ac.LengthSq() - pow(0.5 * (ab.LengthSq() + ac.LengthSq() - bc.LengthSq()), 2);
            minDistSq /= ab.LengthSq();
        }
        return minDistSq;
    }

    float minDistSq(const LineSegment& s)
    {
        Vector3   u = this->mEnd - this->mStart;
        Vector3   v = s.mEnd - s.mStart;
        Vector3   w = this->mStart - s.mStart;
        float    a = Vector3::Dot(u, u);         // always >= 0
        float    b = Vector3::Dot(u, v);
        float    c = Vector3::Dot(v, v);         // always >= 0
        float    d = Vector3::Dot(u, w);
        float    e = Vector3::Dot(v, w);
        float    D = a*c - b*b;        // always >= 0
        float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
        float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

                                    // compute the line parameters of the two closest points
        if (Math::NearZero(D)) { // the lines are almost parallel
            sN = 0.0;         // force using point P0 on segment S1
            sD = 1.0;         // to prevent possible division by 0.0 later
            tN = e;
            tD = c;
        }
        else {                 // get the closest points on the infinite lines
            sN = (b*e - c*d);
            tN = (a*e - b*d);
            if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
                sN = 0.0;
                tN = e;
                tD = c;
            }
            else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
                sN = sD;
                tN = e + b;
                tD = c;
            }
        }

        if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
            tN = 0.0;
            // recompute sc for this edge
            if (-d < 0.0)
                sN = 0.0;
            else if (-d > a)
                sN = sD;
            else {
                sN = -d;
                sD = a;
            }
        }
        else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
            tN = tD;
            // recompute sc for this edge
            if ((-d + b) < 0.0)
                sN = 0;
            else if ((-d + b) > a)
                sN = sD;
            else {
                sN = (-d + b);
                sD = a;
            }
        }
        // finally do the division to get sc and tc
        sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
        tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

        // get the difference of the two closest points
        Vector3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

        return dP.LengthSq();   // return the closest distance squared
    }

    Vector3 PointOnSegment(float t) const
    {
        return mStart + (mEnd - mStart) * t;
    }
};

/**
 * 平面
*/
struct Plane
{
    Vector3 mNormal;
    float mDist;

    Plane(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        Vector3 ab = b - a;
        Vector3 ac = c - a;
        this->mNormal = Vector3::Cross(ab, ac);
        this->mNormal.Normalize();

        this->mDist = -Vector3::Dot(a, this->mNormal);
    }

    float signedDist(const Vector3& c)
    {
        return (Vector3::Dot(c, this->mNormal) - this->mDist);
    }
};

/**
 * 球体
*/
struct Sphere
{
    Vector3 mCenter;
    float mRadius;

    bool contains(const Vector3& point)
    {
        float dist = (this->mCenter - point).LengthSq();
        return dist <= (this->mRadius * this->mRadius);
    }
};

/**
 * 轴对齐包围框
*/
struct AABB
{
    Vector3 mMin;
    Vector3 mMax;

    AABB(const Vector3& min, const Vector3& max)
    {
        this->mMin = min;
        this->mMax = max;
    }

    void updateMinMax(const Vector3& point)
    {
        // 更新包围框的两个点
        mMin.x = Math::Min(mMin.x, point.x);
        mMin.y = Math::Min(mMin.y, point.y);
        mMin.z = Math::Min(mMin.z, point.z);

        mMax.x = Math::Max(mMax.x, point.x);
        mMax.y = Math::Max(mMax.y, point.y);
        mMax.z = Math::Max(mMax.z, point.z);
    }

    void rotate(const Quaternion& q)
    {
        std::array<Vector3, 8> points;
        points[0] = this->mMin;
        points[1] = Vector3(mMax.x, mMin.y, mMin.z);
        points[2] = Vector3(mMin.x, mMax.y, mMin.z);
        points[3] = Vector3(mMin.x, mMin.y, mMax.z);
        points[4] = Vector3(mMax.x, mMax.y, mMin.z);
        points[5] = Vector3(mMax.x, mMin.y, mMax.z);
        points[6] = Vector3(mMin.x, mMax.y, mMax.z);
        points[7] = this->mMax;

        Vector3 p = Vector3::Transform(points[0], q);
        this->mMin = p;
        this->mMax = p;

        for (size_t i = 1; i < points.size(); ++i)
        {
            p = Vector3::Transform(points[i], q);
            this->updateMinMax(p);
        }
    }

    bool contains(const Vector3& point)
    {
        bool outside = (
            point.x < this->mMin.x || 
            point.x > this->mMax.x || 
            point.y < this->mMin.y || 
            point.y > this->mMax.y || 
            point.z < this->mMin.z || 
            point.z > this->mMax.z
        );
        return !outside;
    }

    float minDistSq(const Vector3& point)
    {
        float minDiff = this->mMin.x - point.x;
        float maxDiff = point.x - this->mMax.x;
        float dx = Math::Max(minDiff, 0.0f);
        dx = Math::Max(dx, maxDiff);
        
        minDiff = this->mMin.y - point.y;
        maxDiff = point.y - this->mMax.y;
        float dy = Math::Max(minDiff, 0.0f);
        dy = Math::Max(dy, maxDiff);
          
        minDiff = this->mMin.z - point.z;
        maxDiff = point.z - this->mMax.z;
        float dz = Math::Max(minDiff, 0.0f);
        dz = Math::Max(dz, maxDiff);

        return (dz * dz + dy * dy + dx * dx);
    }
};

/**
 * 定向包围框
*/
struct OBB
{
    Vector3 mCenter;
    Vector3 mExtents;
    float mRotation;
};

/**
 * 胶囊体
*/
struct Capsule
{
    LineSegment mLineSegment;
    float mRadius;

    bool contains(const Vector3& points)
    {
        float distSq = this->mLineSegment.minDistSq(points);
        return distSq <= (this->mRadius * this->mRadius);
    }
};

/**
 * 凸多边形
*/
struct ConvexPolygon
{
    std::vector<Vector2> mVertices;
    
    bool contains(const Vector2& point)
    {
        float sum = 0.0f;
        Vector2 a, b;
        for (size_t i = 0; i < mVertices.size() - 1; i++)
        {
            // From point to first vertex
            a = mVertices[i] - point;
            a.Normalize();
            // From point to second vertex
            b = mVertices[i + 1] - point;
            b.Normalize();
            // Add angle to sum
            sum += Math::Acos(Vector2::Dot(a, b));
        }
        // Have to add angle for last vertex and first vertex
        a = mVertices.back() - point;
        a.Normalize();
        b = mVertices.front() - point;
        b.Normalize();
        sum += Math::Acos(Vector2::Dot(a, b));
        // Return true if approximately 2pi
        return Math::NearZero(sum - Math::TwoPi);
    }
};

/**
 * 球体与球体相交
*/
bool intersect(const Sphere& a, const Sphere& b);

/**
 * 轴对齐包围框与轴对齐包围框相交
*/
bool intersect(const AABB& a, const AABB& b);

/**
 * 球体与轴对齐包围框相交
*/
bool intersect(AABB& a, const Sphere& b);

/**
 * 胶囊体与胶囊体之间的相交
*/
bool intersect(Capsule& a, const Capsule& b);

/**
 * 线段与平面的相交
*/
bool intersect(const LineSegment& l, const Plane& p, float& t);

/**
 * 线段与球体的相交
*/
bool intersect(const LineSegment& l, const Sphere& s, float& t);

/**
 * 线段与面的相交
*/
bool testSidePlane(float start, float end, float negd,
    const Vector3& norm, std::vector<std::pair<float, Vector3>>& out);

/**
 * 线段与包围框的相交
*/
bool intersect(const LineSegment& l, AABB& b, float& outT, Vector3& outNorm);

/**
 * 扫掠球体交叉相交
*/
bool sweptSphere(const Sphere& P0, const Sphere& P1,
    const Sphere& Q0, const Sphere& Q1, float& t);

/**
 * 射线与AABB包围框相交测试
*/
bool intersect(const Ray& ray, const AABB& b, Vector3& inter_point);