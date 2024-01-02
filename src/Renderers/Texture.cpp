#include "../../include/Renderers/Texture.h"

#include <SDL.h>

Texture::Texture(/* args */)
{
    this->mTextureID = 0;
    this->mHeight = 0;
    this->mWidth = 0;
    this->mScale = Vector2{1.0f, 1.0f};
}

Texture::~Texture()
{

}

bool Texture::load(const std::string& fileName, const std::string& type)
{
    this->mTexPath = fileName;
    
    int channels = 0;
    uint8_t* image = SOIL_load_image(
        fileName.c_str(),
        &this->mWidth,
        &this->mHeight,
        &channels,
        SOIL_LOAD_AUTO
    );
    if (!image)
    {
        return false;
    }

    int format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }

    glGenTextures(1, &this->mTextureID);
    glBindTexture(GL_TEXTURE_2D, this->mTextureID);

    // 将原始图像数据复制到OpenGL
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        this->mWidth,
        this->mHeight,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image
    );

    // 完成复制后将图像从内存中释放
    SOIL_free_image_data(image);

    // 启用双线性过滤进行纹理映射
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 启用纹理分级细化
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_FILTER);

    // 打开各向异性过滤
    if (GLEW_EXT_texture_filter_anisotropic)
    {
        // 得到最大的各向异性过滤值
        GLfloat largest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);

        // 使能
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
    }

    return true;
}

void Texture::unLoad()
{
    // 删除纹理
    glDeleteTextures(1, &this->mTextureID);
}

void Texture::setActive(int index)   
{
    // 激活纹理对象
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, this->mTextureID);
}

int Texture::getWidth() const
{
    return this->mWidth;
}

int Texture::getHeight() const
{
    return this->mHeight;
}

void Texture::createFromSurface(SDL_Surface* surface)
{
    this->mWidth = surface->w;
    this->mHeight = surface->h;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    // 创建gl纹理
    if (surface->pixels)
    {
        glGenTextures(1, &this->mTextureID);
        glBindTexture(GL_TEXTURE_2D, this->mTextureID);
        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            GL_RGB, 
            this->mWidth, 
            this->mHeight, 
            0, 
            GL_RGB, 
            GL_UNSIGNED_BYTE, 
            surface->pixels
        );

        // 使用双线性滤波
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        SDL_Log("[Texture] Create surface texture failed...");
    }
}

void Texture::createForRendering(int width, int height, unsigned int format)
{
    this->mWidth = width;
    this->mHeight = height;

    // 创建纹理ID
    glGenTextures(1, &this->mTextureID);
    glBindTexture(GL_TEXTURE_2D, this->mTextureID);

    // 设置图像大小（无初始化）
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        format,
        width,
        height,
        0, 
        GL_RGB,
        GL_FLOAT,
        nullptr  
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

uint32_t Texture::getTextureID() const
{
    return this->mTextureID;
}

const std::string& Texture::getFileName()
{
    return this->mTexPath;
}

const Vector2& Texture::getPosOffset()
{
    return this->mPosOffset;
}

void Texture::setWidth(const int& width)
{
    this->mWidth = width;
}

void Texture::setHeight(const int& height)
{
    this->mHeight = height;
}

void Texture::setTextureID(GLuint texture_id)
{
    this->mTextureID = texture_id;
}

void Texture::setPosOffset(const Vector2& pos_offset)
{
    this->mPosOffset = pos_offset;
}

void Texture::setScale(const Vector2& scale)
{
    this->mScale = scale;
}

const Vector2& Texture::getScale()
{
    return this->mScale;
}

void Texture::setTexName(const std::string& name)
{
    this->mName = name;
}

const std::string& Texture::getTexName() const
{
    return this->mName;
}