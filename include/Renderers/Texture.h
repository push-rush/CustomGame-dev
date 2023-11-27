#pragma once

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

// opengl
#include <glew.h>
#include <glfw3.h>

#include <SOIL.h>

#include "../General/Cmath.h"

#include <ft2build.h>
#include FT_FREETYPE_H 

class Texture
{
private:
    uint32_t mTextureID; // 纹理ID
    int mHeight;
    int mWidth;
    Vector2 mPosOffset;
    std::string mFileName;
    Vector2 mScale;

public:
    Texture(/* args */);
    ~Texture();

    // 加载纹理
    bool load(const std::string& fileName);
    void unLoad();

    // settters
    void setActive(int index = 0);
    void setWidth(const int& width);
    void setHeight(const int& width);
    void setTextureID(GLuint texture_id);
    void setPosOffset(const Vector2& pos_offset);
    void setScale(const Vector2&);

    // getters
    int getWidth() const;
    int getHeight() const;
    uint32_t getTextureID() const;
    const std::string& getFileName();
    const Vector2& getPosOffset();
    const Vector2& getScale(); 

    void createForRendering(int width, int height, unsigned int format);
    void createFromSurface(class SDL_Surface* surface);
};