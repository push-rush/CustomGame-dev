#include "../../include/Renderers/VertexArray.h"
#include <glew.h>
#include <glfw3.h>

VertexArray::VertexArray(const Layout layout, const void* verts, uint32_t numVerts, 
    const uint32_t* indices, uint32_t numIndices)
    : mNumIndices(numIndices), mNumVerts(numVerts)
{
    // 计算顶点字节大小
    unsigned vertSize = sizeof(float) * 8;
    if (layout == PosNormSkinTex)
    {
        vertSize = sizeof(float) * 8 + sizeof(char) * 8;
    }

    /************ 1.创建顶点缓存区 ***************/
    // 创建顶点数组对象
    glGenVertexArrays(1, &this->mVertexArray);
    glBindVertexArray(this->mVertexArray);

    // 创建顶点缓存区
    glGenBuffers(1, &this->mVertexBuffer);
    glBindBuffer(
        GL_ARRAY_BUFFER,   // 指定为顶点缓存区
        this->mVertexBuffer
    );

    // 将顶点数据复制到顶点缓存区
    glBufferData(
        GL_ARRAY_BUFFER,  // 指定写入缓存区类型（顶点缓存区）
        numVerts * vertSize,
        verts,
        GL_STATIC_DRAW    // 指定代码如何使用缓存区，此参数表示数据加载一次，反复使用
    );

    /************ 2.创建索引缓存区 *************/
    // 创建索引缓存区
    glGenBuffers(
        1, 
        &this->mIndiceBuffer
    );

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER, 
        this->mIndiceBuffer
    );

    // 将索引数据复制到索引缓存区
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        numIndices * sizeof(uint32_t),
        indices,
        GL_STATIC_DRAW
    );

    if (layout == PosNormTex)
    {
        /************* 3.指定顶点布局 *******************/
        // 启用第一个顶点属性
        glEnableVertexAttribArray(0);
        // 设置属性
        glVertexAttribPointer(
            0,  
            3,
            GL_FLOAT,
            GL_FALSE,
            vertSize,
            0
        );

        // 启用第二个顶点属性
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,  // 槽号
            3,
            GL_FLOAT,
            GL_FALSE,
            vertSize,          // 步幅 
            reinterpret_cast<void*>(sizeof(float) * 3)  // 指针偏移
        );

        // 启用第三个属性
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            vertSize,
            reinterpret_cast<void*>(sizeof(float) * 6)
        );
    }
    else if (layout == PosNormSkinTex)
    {
        // 定义顶点属性
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 
            3, 
            GL_FLOAT, 
            GL_FALSE, 
            vertSize, 
            nullptr
        );

        // 法线属性
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 
            3,
            GL_FLOAT,
            GL_FALSE,
            vertSize,
            reinterpret_cast<void*>(sizeof(float) * 3)
        );

        // 影响顶点的骨骼属性
        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(
            2,
            4,
            GL_UNSIGNED_BYTE,
            vertSize,
            reinterpret_cast<void*>(sizeof(float) * 6)
        );

        // 影响顶点的骨骼权重属性
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
            3,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,
            vertSize,
            reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4)
        );

        // 纹理坐标属性
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(
            4,
            2,
            GL_FLOAT,
            GL_FALSE,
            vertSize,
            reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8)
        );
    }
}

VertexArray::~VertexArray()
{
    glDeleteBuffers(1, &this->mVertexBuffer);
    glDeleteBuffers(1, &this->mIndiceBuffer);
    glDeleteVertexArrays(1, &this->mVertexArray);
}

void VertexArray::setActive()
{
    // 激活当前正在使用的顶点数组
    glBindVertexArray(this->mVertexArray);
}

uint32_t VertexArray::getNumIndices() const
{
    return this->mNumIndices;
}

uint32_t VertexArray::getNumVerts() const
{
    return this->mNumVerts;
}