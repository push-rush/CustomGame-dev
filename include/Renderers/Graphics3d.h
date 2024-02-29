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
        static bool drawCube(class Shader* shader, const Vector3& left_up, const float& width, const float& height, const float& length, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f});

        static bool drawCube(class Shader* shader, const Vector3& center, const float& width, const float& height, const float& length,
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, bool other = true);
        
        // 球体
        static bool drawSphere(Shader* shader, const Vector3& center, const float& radius, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& slices = 6, const int& stacks = 12);
        
        // 圆锥
        static bool drawCone(Shader* shader, const Vector3& center, const float& radius, const float& height, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& slices = 3);
        
        // 圆柱
        static bool drawCylinder(Shader* shader, const Vector3& center, const float& radius, const float& height, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& partition = 3);
        
        // 平面
        static bool drawPlane(Shader* shader, const Vector3& center, const float& width, const float& length, const float& height = 0.0f,
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f});
    };

} // namespace Renderers