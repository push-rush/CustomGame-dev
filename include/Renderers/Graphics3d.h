#pragma once

#include <iostream>
#include <vector>

#include "../General/Cmath.h"

#include "./Shader.h"

namespace Renderers
{
    class Graphics3d
    {
    private:


    public:
        Graphics3d();
        ~Graphics3d();

        // 立方体
        static bool drawCube(class Shader* shader, const Vector2& start, const Vector2& end, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        
        // 球体
        static bool drawSphere(Shader* shader, const Vector2& center, const int& radius, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        
        // 圆锥
        static bool drawCone(Shader* shader, const Vector2& center, const float& width, const float& height, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        
        // 圆柱
        static bool drawCylinder(Shader* shader, const Vector2& center, const float& width, const float& height, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        
        // 平面
        static bool drawPlane(Shader* shader, const Vector2& center, const float& width, const float& height, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
    };

} // namespace Renderers