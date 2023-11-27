#include "../../include/Renderers/Collision.h"

bool intersect(const Sphere& a, const Sphere& b)
{
    float distSq = (a.mCenter - b.mCenter).LengthSq();
    float sumRadius = (a.mRadius + b.mRadius);
    return distSq <= (sumRadius * sumRadius);
}

bool intersect(const AABB& a, const AABB& b)
{
    bool no = (a.mMax.x < b.mMin.x ||
        a.mMax.y < b.mMin.y ||
        a.mMax.z < b.mMin.z ||
        a.mMin.x > b.mMax.x ||
        a.mMin.y > b.mMax.y ||
        a.mMin.z > b.mMax.z
    );
    return !no;
}

bool intersect(AABB& a, const Sphere& b)
{
    float distSq = a.minDistSq(b.mCenter);
    return distSq <= (b.mRadius * b.mRadius);
}

bool intersect(Capsule& a, const Capsule& b)
{
    float distSq = a.mLineSegment.minDistSq(b.mLineSegment);
    float sumRadius = a.mRadius + b.mRadius;
    return distSq <= (sumRadius * sumRadius);
}

bool intersect(const LineSegment& l, const Plane& p, float& t)
{
    // 线段向量与平面法线点积判断t是否有解
    float denom = Vector3::Dot(l.mEnd - l.mStart, p.mNormal);
    if (Math::NearZero(denom))
    {   // 线段与平面平行，判断线段是否在平面上
        if (Math::NearZero(Vector3::Dot(l.mEnd, p.mNormal) - p.mDist))
        {
            t = 0.0f;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        float numer = -Vector3::Dot(l.mStart, p.mNormal) - p.mDist;
        t = numer / denom;
        if (t >= 0.0f && t <= 1.0f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool intersect(const LineSegment& l, const Sphere& s, float& t)
{   
    // Compute X, Y, a, b, c as per equations
	Vector3 X = l.mStart - s.mCenter;
	Vector3 Y = l.mEnd - l.mStart;
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float c = Vector3::Dot(X, X) - s.mRadius * s.mRadius;
	// Compute discriminant
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
		// Compute min and max solutions of t
		float tMin = (-b - disc) / (2.0f * a);
		float tMax = (-b + disc) / (2.0f * a);
		// Check whether either t is within bounds of segment
		if (tMin >= 0.0f && tMin <= 1.0f)
		{
			t = tMin;
			return true;
		}
		else if (tMax >= 0.0f && tMax <= 1.0f)
		{
			t = tMax;
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool testSidePlane(float start, float end, float negd, const Vector3& norm, std::vector<std::pair<float, Vector3>>& out)
{
    float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		// Test that t is within bounds
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool intersect(const LineSegment& l, AABB& b, float& outT, Vector3& outNorm)
{
    // Vector to save all possible t values, and normals for those sides
	std::vector<std::pair<float, Vector3>> tValues;
	// Test the x planes
	testSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3::NegUnitX,
		tValues);
	testSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3::UnitX,
		tValues);
	// Test the y planes
	testSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3::NegUnitY,
		tValues);
	testSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3::UnitY,
		tValues);
	// Test the z planes
	testSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3::NegUnitZ,
		tValues);
	testSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3::UnitZ,
		tValues);
	
	// Sort the t values in ascending order
	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b) {
		return a.first < b.first;
	});

	// Test if the box contains any of these points of intersection
	Vector3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	//None of the intersections are within bounds of box
	return false;
}

bool sweptSphere(const Sphere& P0, const Sphere& P1,
    const Sphere& Q0, const Sphere& Q1, float& t)
{
    // Compute X, Y, a, b, and c
	Vector3 X = P0.mCenter - Q0.mCenter;
	Vector3 Y = P1.mCenter - P0.mCenter -
		(Q1.mCenter - Q0.mCenter);
	float a = Vector3::Dot(Y, Y);
	float b = 2.0f * Vector3::Dot(X, Y);
	float sumRadii = P0.mRadius + Q0.mRadius;
	float c = Vector3::Dot(X, X) - sumRadii * sumRadii;
	// Solve discriminant
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
		// We only care about the smaller solution
		t = (-b - disc) / (2.0f * a);
		if (t >= 0.0f && t <= 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}