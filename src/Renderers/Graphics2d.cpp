#include "../../include/Renderers/Graphics2d.h"

#include "../../include/Renderers/FreeTypeFont.h"
#include "../../include/Renderers/VertexArray.h"
#include "../../include/Renderers/Texture.h"

#include "../../include/Game.h"

namespace Renderers
{

Graphics2d::Graphics2d()
{

}

Graphics2d::~Graphics2d()
{

}

// 线段
bool Graphics2d::drawLine(Shader* shader, const Vector2& start, const Vector2& end, 
    const Vector3& color, const int& thickness)
{
    if (shader)
    {
        float verts[] = {
            start.x, start.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            end.x,   end.y,   0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
        };

        uint32_t indexBuffer[] = {
            0, 1
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 2, indexBuffer, 2);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glLineWidth(thickness);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}

// 圆
bool Graphics2d::drawCircle(Shader* shader, const Vector2& center, const int& radius, 
    const Vector3& color, const int& thickness)
{
    if (shader)
    {
        float verts[] = {
            center.x, center.y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };

        uint32_t indexBuffer[] = {
            0
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 1, indexBuffer, 1);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        if (!glIsEnabled(GL_POINT_SMOOTH))
            glEnable(GL_POINT_SMOOTH);
        
        glPointSize(radius);
        glDrawElements(GL_LINES, 1, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}

// 矩形
bool Graphics2d::drawRectangle(Shader* shader, const Vector2& center, const float& width, const float& height,
    const Vector3& color, const int& thickness)
{
    if (shader)
    {
        float verts[] = {
            center.x - width * 0.5, center.y + height * 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x + width * 0.5, center.y + height * 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            center.x + width * 0.5, center.y - height * 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            center.x - width * 0.5, center.y - height * 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
        };

        uint32_t indexBuffer[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 4, indexBuffer, 6);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool Graphics2d::drawRectangle(Shader* shader, const Vector2& left_up, const Vector2& right_down, 
    const Vector3& color, const int& thickness)
{
    if (shader)
    {
        float verts[] = {
            left_up.x, left_up.y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            right_down.x, left_up.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            right_down.x, right_down.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            left_up.x, right_down.y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
        };

        uint32_t indexBuffer[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 4, indexBuffer, 6);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}

// 三角形
bool Graphics2d::drawTriangle(Shader* shader, const Vector2& p1, const Vector2& p2, const Vector2& p3, 
    const Vector3& color, const int& thickness)
{
    if (shader)
    {
        float verts[] = {
            p1.x, p1.y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            p2.x, p2.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            p3.x, p3.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
        };

        uint32_t indexBuffer[] = {
            0, 1, 2
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 3, indexBuffer, 3);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }


    return true;
}

bool Graphics2d::drawTriangle(Shader* shader, const std::vector<Vector2>& points, 
    const Vector3& color, const int& thickness)
{
    if (shader && ((int)points.size()) == 3)
    {
        auto iter = points.begin();
        float verts[] = {
            (iter)->x, (iter)->y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            (iter+1)->x, (iter+1)->y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            (iter+2)->x, (iter+2)->y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
        };

        uint32_t indexBuffer[] = {
            0, 1, 2
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 3, indexBuffer, 3);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool Graphics2d::drawTriangle(Shader* shader, const Vector2*& points, 
    const Vector3& color, const int& thickness)
{
    if (shader)
    {
        float verts[] = {
            (*points).x, (*points).y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            (*(points+1)).x, (*(points+1)).y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            (*(points+2)).x, (*(points+2)).y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
        };

        uint32_t indexBuffer[] = {
            0, 1, 2
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 3, indexBuffer, 3);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}                

// 折线
bool Graphics2d::drawPolylines(Shader* shader, const std::vector<Vector2>& points, 
    const Vector3& color, const int& thickness)
{
    if (shader && (int)(points.size()) > 1)
    {
        float verts[(int)(points.size()) * 8] = { 0 };
        uint32_t indexBuffer[(int)(points.size()) * 2 - 2] = { 0 }; 

        for (int i = 0; i < (int)(points.size()); ++i)
        {
            verts[i * 8 + 0] = points[i].x;
            verts[i * 8 + 1] = points[i].y;
            verts[i * 8 + 2] = 0.0f;
            verts[i * 8 + 3] = 0.0f;
            verts[i * 8 + 4] = 0.0f;
            verts[i * 8 + 5] = 1.0f;
            verts[i * 8 + 6] = 0.0f;
            verts[i * 8 + 7] = 0.0f;
            
            if (i > 0 || i < ((int)points.size() - 1))
            {
                indexBuffer[i * 2 - 1] = i;
                indexBuffer[i * 2] = i;
            }
            else if (i == 0)
            {
                indexBuffer[i] = i;
            }
            else if (i == ((int)points.size() - 1))
            {
                indexBuffer[i * 2 - 1] = i;
            }
        }

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, (size_t)points.size(), indexBuffer, (int)(points.size()) * 2 - 2);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glLineWidth(thickness);
        glDrawElements(GL_LINES, (int)(points.size()) * 2 - 2, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}                

// 多边形
bool Graphics2d::drawPolygon(Shader* shader, const std::vector<Vector2>& points,
    const Vector3& color, const int& thickness)
{
    if (shader && (int)(points.size()) > 2)
    {
        float verts[(int)(points.size()) * 8] = { 0 };
        uint32_t indexBuffer[(int)(points.size()) * 2] = { 0 }; 

        for (int i = 0; i < (int)(points.size()); ++i)
        {
            verts[i * 8 + 0] = points[i].x;
            verts[i * 8 + 1] = points[i].y;
            verts[i * 8 + 2] = 0.0f;
            verts[i * 8 + 3] = 0.0f;
            verts[i * 8 + 4] = 0.0f;
            verts[i * 8 + 5] = 1.0f;
            verts[i * 8 + 6] = 0.0f;
            verts[i * 8 + 7] = 0.0f;
            
            if (i > 0 || i < ((int)points.size() - 1))
            {
                indexBuffer[i * 2 - 1] = i;
                indexBuffer[i * 2] = i;
            }
            else if (i == 0)
            {
                indexBuffer[0] = 0;
            }
            else if (i == ((int)points.size() - 1))
            {
                indexBuffer[i * 2 - 1] = i;
                indexBuffer[i * 2] = i;
                indexBuffer[i * 2 + 1] = 0;
            }
        }

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, (size_t)points.size(), indexBuffer, (int)(points.size()) * 2);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glLineWidth(thickness);
        glDrawElements(GL_LINES, (int)(points.size()) * 2, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}                

// 箭头
bool Graphics2d::drawArrowhead(Shader* shader, const Vector2& center, const Vector2& size,
    const Vector3& color, const int& thickness)
{
    if (shader)
    {
        float verts[] = {
            center.x - size.x * 0.5, center.y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x, center.y + size.y * 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            center.x + size.x * 0.5, center.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            center.x - size.x * 0.25, center.y, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x + size.x * 0.25, center.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            center.x + size.x * 0.25, center.y - size.y * 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            center.x - size.x * 0.25, center.y - size.y * 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        };

        uint32_t indexBuffer[] = {
            0, 1, 2,
            3, 4, 5,
            5, 6, 3
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 7, indexBuffer, 9);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);

        if (v)
        {
            delete v;
            v = nullptr;
        }
    }
    else
    {
        return false;
    }

    return true;
}

// 绘制文本
// bool Graphics2d::drawText(UIScreen* ui, Shader* shader, const std::wstring& text, const Vector2& pos, const int& fontSize, 
//     const Vector3& color, const int& thickness)
// {
//     auto freeTypeFont = ui->getGame()->getFreeTypeFont();
//     std::vector<class Texture*> texs;

//     Vector2 size = freeTypeFont->renderText(texs, text.c_str(), color, fontSize);

//     if (texs.size() > 0)
//     {
//         float dx = -0.5 * size.x;
//         float dy = 0.5 * size.y;
        
//         // 激活文本着色器
//         shader->setActive();

//         // 设置文本颜色
//         shader->setVectorUniform("uColor", color);

//         // 绘制按钮文本
//         ui->drawTextures(shader, texs, 
//             Vector2{pos.x + dx, pos.y - dy}, 
//             1.0f, 1.0f, false, true
//         );
//     }
//     else
//     {
//         return false;
//     }

//     return true;
// }

} // namespace Renderers