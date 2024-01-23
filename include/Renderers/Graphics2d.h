#pragma once

#include <iostream>
#include <string.h>
#include <vector>

#include "../General/Cmath.h"

#include "./Shader.h"
#include "./UIScreen.h"

namespace Renderers
{
    class Graphics2d
    {
    private:

    public:
        Graphics2d();
        ~Graphics2d();

        // 线段
        static bool drawLine(class Shader* shader, const Vector2& start, const Vector2& end, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        
        // 圆
        static bool drawCircle(Shader* shader, const Vector2& center, const int& radius, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        
        // 矩形
        static bool drawRectangle(Shader* shader, const Vector2& center, const float& width, const float& height, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        static bool drawRectangle(Shader* shader, const Vector2& left_up, const Vector2& right_down, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        
        // 三角形
        static bool drawTriangle(Shader* shader, const Vector2& p1, const Vector2& p2, const Vector2& p3, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);
        static bool drawTriangle(Shader* shader, const std::vector<Vector2>& points, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);                
        static bool drawTriangle(Shader* shader, const Vector2*& points, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);                
        
        // 折线
        static bool drawPolylines(Shader* shader, const std::vector<Vector2>& points, 
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);                
        
        // 多边形
        static bool drawPolygon(Shader* shader, const std::vector<Vector2>& points,
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);                
        
        // 箭头
        static bool drawArrowhead(Shader* shader, const Vector2& center, const Vector2& size,
                        const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);    

        // 文本
        // static bool drawText(class UIScreen* ui, Shader* shader, const std::wstring& text, const Vector2& pos, const int& fontSize, 
        //                 const Vector3& color = Vector3{0.85f, 0.85f, 0.85f}, const int& thickness = 5);            
    };

} //Renderers

