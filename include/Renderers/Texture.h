/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:38:25
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 20:19:11
 * @FilePath: /CustomGame-dev/include/Renderers/Texture.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

// opengl
#include <GL/glew.h>

#include <SOIL/SOIL.h>

#include <SDL2/SDL.h>

#include "../General/Cmath.h"

// #include <ft2build.h>
// #include FT_FREETYPE_H 

#define DefaultTextureName "Texture"

class Texture
{
private:
    int mHeight;
    int mWidth;
    std::string mName;
    Vector2 mPosOffset;
    Vector2 mScale;
    std::string mTexPath;
    uint32_t mTextureID; // 纹理ID

public:
    Texture();
    Texture(const std::string& tex_name);
    ~Texture();

    // 加载纹理
    bool load(const std::string& fileName, const std::string& type = "default");
    void unLoad();

    // settters
    void setActive(int index = 0);
    void setWidth(const int& width);
    void setHeight(const int& width);
    void setTextureID(GLuint texture_id);
    void setPosOffset(const Vector2& pos_offset);
    void setScale(const Vector2&);
    void setTexName(const std::string& type);
    
    // getters
    int getWidth() const;
    int getHeight() const;
    uint32_t getTextureID() const;
    const std::string& getFileName();
    const Vector2& getPosOffset();
    const Vector2& getScale(); 
    const std::string& getTexName() const;

    void createForRendering(int width, int height, unsigned int format);
    void createFromSurface(class SDL_Surface* surface);
};