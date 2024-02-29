#include "../../include/Renderers/Graphics3d.h"

#include "../../include/Renderers/VertexArray.h"

namespace Renderers
{

Graphics3d::Graphics3d()
{

}

Graphics3d::~Graphics3d()
{

}

// 立方体
bool Graphics3d::drawCube(class Shader* shader, const Vector3& left_up, const float& width, const float& height, const float& length, 
    const Vector3& color)
{
    if (shader)
    {
        float verts[] = {
            left_up.x, left_up.y, left_up.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            left_up.x + width, left_up.y, left_up.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            left_up.x + width, left_up.y - height, left_up.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            left_up.x, left_up.y - height, left_up.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            left_up.x, left_up.y, left_up.z + length, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            left_up.x + width, left_up.y, left_up.z + length, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            left_up.x + width, left_up.y - height, left_up.z + length, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            left_up.x, left_up.y - height, left_up.z + length, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        };

        uint32_t indexBuffer[] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 3, 7,
            7, 4, 0,
            1, 2, 6,
            6, 5, 1,
            0, 1, 5,
            5, 4, 0,
            3, 2, 6,
            6, 7, 3
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 8, indexBuffer, 3 * 12);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, nullptr);

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

bool Graphics3d::drawCube(class Shader* shader, const Vector3& center, const float& width, const float& height, const float& length,
    const Vector3& color, bool other)
{
    if (shader)
    {
        float verts[] = {
            center.x - width * 0.5f, center.y + height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y + height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y - height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            center.x - width * 0.5f, center.y - height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            center.x - width * 0.5f, center.y + height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y + height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y - height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x - width * 0.5f, center.y - height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  
        };

        uint32_t indexBuffer[] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 3, 7,
            7, 4, 0,
            1, 2, 6,
            6, 5, 1,
            0, 1, 5,
            5, 4, 0,
            3, 2, 6,
            6, 7, 3
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 8, indexBuffer, 3 * 12);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, nullptr);

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

// 圆柱
bool Graphics3d::drawCylinder(Shader* shader, const Vector3& center, const float& radius, const float& height, 
    const Vector3& color, const int& partition_num)
{
    if (shader)
    {
        std::vector<float> verts_top;
        std::vector<float> verts_bot;

        float center_top[] = {center.x, center.y + height * 0.5f, center.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
        // float center_top[] = {center.x, center.z, center.y + height * 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
        float center_bot[] = {center.x, center.y - height * 0.5f, center.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f};
        // float center_bot[] = {center.x, center.z, center.y - height * 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f};
        
        verts_top.emplace_back(center_top[0]);
        verts_top.emplace_back(center_top[1]);
        verts_top.emplace_back(center_top[2]);
        verts_top.emplace_back(center_top[3]);
        verts_top.emplace_back(center_top[4]);
        verts_top.emplace_back(center_top[5]);
        verts_top.emplace_back(center_top[6]);
        verts_top.emplace_back(center_top[7]);

        verts_bot.emplace_back(center_bot[0]);
        verts_bot.emplace_back(center_bot[1]);
        verts_bot.emplace_back(center_bot[2]);
        verts_bot.emplace_back(center_bot[3]);
        verts_bot.emplace_back(center_bot[4]);
        verts_bot.emplace_back(center_bot[5]);
        verts_bot.emplace_back(center_bot[6]);
        verts_bot.emplace_back(center_bot[7]);

        const float sector_angle = 2 * Math::Pi / partition_num;
        for (int i = 0; i < partition_num; i++)
        {
            float cur_angle = sector_angle * i;
            Vector3 pos_top = Vector3{center.x + radius * Math::Cos(cur_angle), center.y + height * 0.5f, center.z + radius * Math::Sin(cur_angle)};
            Vector3 pos_bot = Vector3{center.x + radius * Math::Cos(cur_angle), center.y - height * 0.5f, center.z + radius * Math::Sin(cur_angle)};
            Vector3 normal = Vector3{Math::Cos(cur_angle), 0.0f, Math::Sin(cur_angle)};

            verts_top.emplace_back(pos_top.x);
            verts_top.emplace_back(pos_top.y);
            // verts_top.emplace_back(pos_top.z);
            verts_top.emplace_back(pos_top.z);
            // verts_top.emplace_back(pos_top.y);

            verts_top.emplace_back(normal.x);
            verts_top.emplace_back(normal.y);
            verts_top.emplace_back(normal.z);
            verts_top.emplace_back(0.0f);
            verts_top.emplace_back(1.0f);

            verts_bot.emplace_back(pos_bot.x);
            verts_bot.emplace_back(pos_bot.y);
            // verts_bot.emplace_back(pos_bot.z);
            verts_bot.emplace_back(pos_bot.z);
            // verts_bot.emplace_back(pos_bot.y);

            verts_bot.emplace_back(normal.x);
            verts_bot.emplace_back(normal.y);
            verts_bot.emplace_back(normal.z);
            verts_bot.emplace_back(0.0f);
            verts_bot.emplace_back(1.0f);
        }

        // 合并上下底面顶点
        std::vector<float> verts;
        verts.insert(verts.end(), verts_top.begin(), verts_top.end());
        verts.insert(verts.end(), verts_bot.begin(), verts_bot.end());

        // 添加顶点索引
        std::vector<uint32_t> indices;
        for (int i = 1; i < partition_num + 1; i++)
        {
            auto temp_idx1 = (i + 1) % (partition_num + 1) == 0 ? (i + 1) : 1;
            auto temp_idx2 = (partition_num + 1 + i + 1) % (2 * (partition_num + 1)) == 0 ? (partition_num + 1 + i + 1) : (partition_num + 2);
            
            // 上底面顶点索引
            indices.emplace_back(0);
            indices.emplace_back(i);
            indices.emplace_back(temp_idx1);

            // 下底面顶点索引
            indices.emplace_back(partition_num + 1);
            indices.emplace_back(partition_num + 1 + i);
            indices.emplace_back(temp_idx2);

            // 侧面顶点索引
            indices.emplace_back(i);
            indices.emplace_back(partition_num + 1 + i);
            indices.emplace_back(temp_idx2);

            indices.emplace_back(i);
            indices.emplace_back(temp_idx1);
            indices.emplace_back(temp_idx2);
        }
        
        // SDL_Log("[Graphics] vertex size: %d index size: %d", verts.size(), indices.size());
        
        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts.data(), (partition_num + 1) * 2, indices.data(), indices.size());
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        // triangles size: partition_num * 4
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

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

// 球体
bool Graphics3d::drawSphere(Shader* shader, const Vector3& center, const float& radius, 
    const Vector3& color, const int& slices, const int& stacks)
{
    if (shader)
    {
        std::vector<float> verts;
        
        // 球体最高点
        verts.emplace_back(center.x);
        verts.emplace_back(center.y + radius);
        verts.emplace_back(center.z);
        verts.emplace_back(0.0f);
        verts.emplace_back(1.0f);
        verts.emplace_back(0.0f);
        verts.emplace_back(0.0f);
        verts.emplace_back(1.0f);

        for (int i = slices - 1; i > 0; i--)
        {
            float lat = Math::Pi * (-0.5f + (i / slices));
            float y0 = Math::Sin(lat);
            float yr0 = Math::Cos(lat);

            for (int j = 0; j < stacks; j++)
            {
                float lng = 2.0f * Math::Pi * (j / stacks);
                float x = Math::Cos(lng);
                float z = Math::Sin(lng);

                verts.emplace_back(center.x + x * yr0 * radius);
                verts.emplace_back(center.y + y0 * radius);
                verts.emplace_back(center.z + z * yr0 * radius);
                
                verts.emplace_back(x * yr0);
                verts.emplace_back(y0);
                verts.emplace_back(z * yr0);
                verts.emplace_back(0.0f);
                verts.emplace_back(1.0f);
            }
        }

        // 球体最低点
        verts.emplace_back(center.x);
        verts.emplace_back(center.y - radius);
        verts.emplace_back(center.z);
        verts.emplace_back(0.0f);
        verts.emplace_back(-1.0f);
        verts.emplace_back(0.0f);
        verts.emplace_back(0.0f);
        verts.emplace_back(1.0f);

        // 顶点索引序列
        std::vector<uint32_t> indices;
        
        int end = verts.size() - 1;
        for (int i = 1; i < stacks + 1; i++)
        {
            indices.emplace_back(0);
            indices.emplace_back(i);
            indices.emplace_back((i + 1) % stacks);

            indices.emplace_back(end);
            indices.emplace_back(end - i);
            indices.emplace_back(end - ((i + 1) % stacks));
        }

        for (int i = 0; i < slices - 2; i++)
        {
            for (int j = 0; j < stacks; j++)
            {
                indices.emplace_back(i * stacks + 1 + j);
                indices.emplace_back((i + 1) * stacks + 1 + j);
                indices.emplace_back((i + 1) * stacks + 1 + (j + 1) % stacks);

                indices.emplace_back(i * stacks + 1 + j);
                indices.emplace_back(i * stacks + 1 + (j + 1) % stacks);
                indices.emplace_back((i + 1) * stacks + 1 + (j + 1) % stacks);
            }
        }

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts.data(), (slices - 2) * stacks + 2, indices.data(), indices.size());
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        // triangles size: slices * stacks
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

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

// 圆锥
bool Graphics3d::drawCone(Shader* shader, const Vector3& center, const float& radius, const float& height, 
    const Vector3& color, const int& slices)
{
    if (shader)
    {
        std::vector<float> verts;

        // 圆锥顶点
        verts.emplace_back(center.x);
        verts.emplace_back(center.y + height);
        verts.emplace_back(center.z);
        verts.emplace_back(0.0f);
        verts.emplace_back(1.0f);
        verts.emplace_back(0.0f);
        verts.emplace_back(1.0f);
        verts.emplace_back(0.0f);

        for (int i = 0; i < slices; i++)
        {
            float angle = 2.0f * Math::Pi * (i / slices);

            float x = Math::Cos(angle);
            float y = Math::Sin(angle);

            verts.emplace_back(center.x + radius * x);
            verts.emplace_back(center.y);
            verts.emplace_back(center.z + radius * y);
            verts.emplace_back(x);
            verts.emplace_back(0.0f);
            verts.emplace_back(y);
            verts.emplace_back(1.0f);
            verts.emplace_back(0.0f);
        }

        std::vector<uint32_t> indices;
        for (int i = 1; i < slices + 1; i++)
        {
            indices.emplace_back(0);
            indices.emplace_back(i);
            indices.emplace_back((i + 1) % slices);
        }

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts.data(), slices + 1, indices.data(), indices.size());
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        // triangles size: slices
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

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

// 平面
bool Graphics3d::drawPlane(Shader* shader, const Vector3& center, const float& width, const float& length, const float& height, const Vector3& color)
{
    if (shader)
    {
        float verts[] = {
            center.x - width * 0.5f, center.y + height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y + height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y - height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            center.x - width * 0.5f, center.y - height * 0.5f, center.z - length * 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            center.x - width * 0.5f, center.y + height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y + height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x + width * 0.5f, center.y - height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            center.x - width * 0.5f, center.y - height * 0.5f, center.z + length * 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  
        };

        uint32_t indexBuffer[] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 3, 7,
            7, 4, 0,
            1, 2, 6,
            6, 5, 1,
            0, 1, 5,
            5, 4, 0,
            3, 2, 6,
            6, 7, 3
        };

        VertexArray* v = new VertexArray(VertexArray::PosNormTex, verts, 8, indexBuffer, 3 * 12);
        
        shader->setActive();
        v->setActive();

        Matrix4 scaleMat = Matrix4::CreateScale(1.0f, 1.0f, 1.0f);
        Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
        Matrix4 world = scaleMat * transMat;
        shader->setMatrixUniform("uWorldTransform", world);
        shader->setVectorUniform("uColor", color);

        glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, nullptr);

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

} //namespace Renderers

