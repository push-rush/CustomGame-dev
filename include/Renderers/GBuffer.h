#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class GBuffer
{
public:
    GBuffer(/* args */);
    ~GBuffer();

    enum Type
    {
        EDiffuse = 0,
        ENormal,
        EWorldPos,
        ESpecular,
        NUM_GBUFFER_TEXTURES,
    };

    // 创建G缓存区
    bool create(int width, int height);
    void destroy();

    // 返回指定类型的纹理
    class Texture* getTexture(Type type);

    // 返回缓存区对象ID
    unsigned int getBufferID() const;

    // 设置所有采样的纹理
    void setTexturesActive();

private:
    // 与G缓存区关联的纹理
    std::vector<class Texture*> mTextures;

    // 帧缓存区对象ID
    unsigned int mBufferID;
};  