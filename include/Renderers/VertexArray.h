#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

class VertexArray
{
public:
    enum Layout
    {
        PosNormTex,
        PosNormSkinTex
    };

public:
    VertexArray(const Layout layout, const void* verts, uint32_t numVerts,
        const uint32_t* indices, uint32_t numIndices);
    ~VertexArray();

    // 激活此顶点数组
    void setActive();

    uint32_t getNumIndices() const;
    uint32_t getNumVerts() const;

private:
    uint32_t mNumVerts;
    uint32_t mNumIndices;
    uint32_t mVertexBuffer;
    uint32_t mIndiceBuffer;
    uint32_t mVertexArray;
};