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
    int mHeight;
    int mWidth;
    std::string mName;
    Vector2 mPosOffset;
    Vector2 mScale;
    std::string mTexPath;
    uint32_t mTextureID; // 纹理ID

public:
    Texture(/* args */);
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