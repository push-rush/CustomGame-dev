/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:38:25
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 20:18:58
 * @FilePath: /CustomGame-dev/include/Renderers/VertexArray.h
 * @Description: Custom game engine constructing...
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */

#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

class VertexArray
{
public:
    enum Layout
    {
        PosNormTex,
        PosNormSkinTex
    };

public:
    VertexArray();
    VertexArray(const VertexArray& other);
    VertexArray operator =(const VertexArray& other)
    {
        this->mVertexArray = other.mVertexArray;
        this->mVertexBuffer = other.mVertexBuffer;
        this->mIndiceBuffer = other.mIndiceBuffer;
        this->mNumVerts = other.mNumVerts;
        this->mNumIndices = other.mNumIndices;
        
        return *this;
    }

    VertexArray(const Layout layout, const void* verts, uint32_t numVerts,
        const uint32_t* indices, uint32_t numIndices);
    ~VertexArray();

    // 激活此顶点数组
    void setActive();

    uint32_t getNumIndices() const;
    uint32_t getNumVerts() const;
    uint32_t getVertexBuffer() const;
    uint32_t getIndiceBuffer() const;
    uint32_t getVertexArray() const;

private:
    uint32_t mNumVerts;
    uint32_t mNumIndices;
    uint32_t mVertexBuffer;
    uint32_t mIndiceBuffer;
    uint32_t mVertexArray;
};