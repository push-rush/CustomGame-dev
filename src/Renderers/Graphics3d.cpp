#include "../../include/Renderers/Graphics3d.h"

namespace Renderers
{

Graphics3d::Graphics3d()
{

}

Graphics3d::~Graphics3d()
{

}

// 立方体
bool Graphics3d::drawCube(class Shader* shader, const Vector2& start, const Vector2& end, 
    const Vector3& color, const int& thickness)
{

    return true;
}

// 球体
bool Graphics3d::drawSphere(Shader* shader, const Vector2& center, const int& radius, 
    const Vector3& color, const int& thickness)
{
    return true;
    
}

// 圆锥
bool Graphics3d::drawCone(Shader* shader, const Vector2& center, const float& width, const float& height, 
    const Vector3& color, const int& thickness)
{
    return true;
    
}

// 圆柱
bool Graphics3d::drawCylinder(Shader* shader, const Vector2& center, const float& width, const float& height, 
    const Vector3& color, const int& thickness)
{
    return true;

}

// 平面
bool Graphics3d::drawPlane(Shader* shader, const Vector2& center, const float& width, const float& height, 
    const Vector3& color, const int& thickness)
{
    return true;

}



} //namespace Renderers

