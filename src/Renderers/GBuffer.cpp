#include "../../include/Renderers/GBuffer.h"
#include "../../include/Renderers/Texture.h"

#include <SDL2/SDL.h>

GBuffer::GBuffer()
{
    this->mBufferID = 0;
}

GBuffer::~GBuffer()
{

}

bool GBuffer::create(int width, int height)
{
    // 创建帧缓存对象并保存ID
	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
    
    // 为目标添加深度缓存区
	// GLuint depthBuffer;
	// glGenRenderbuffers(1, &depthBuffer);
	// glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    // 为目标添加模板测试缓存区
    // GLuint stencilBuffer;
    // glGenRenderbuffers(1, &stencilBuffer);
    // glBindRenderbuffer(GL_RENDERBUFFER, stencilBuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_COMPONENTS, width, height);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer);

    GLuint depthStencilBuffer;
    glGenRenderbuffers(1, &depthStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer);

    // 为G缓存区的每一个输出创建纹理
    for (int i = 0; i < NUM_GBUFFER_TEXTURES; ++i)
    {
        Texture* tex = new Texture();
        tex->createForRendering(width, height, GL_RGB32F);
        this->mTextures.emplace_back(tex);

        // 添加纹理到颜色输出
        glFramebufferTexture(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            tex->getTextureID(),
            0
        );
    }

    // 创建颜色附件的容器
    std::vector<GLenum> attachments;
    for (int i = 0; i < NUM_GBUFFER_TEXTURES; ++i)
    {
        attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
    }

    // 设置要绘制的缓存区列表
    glDrawBuffers(
        static_cast<GLsizei>(attachments.size()),
        attachments.data()
    );

    // 判断任务流程是否正常
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        this->destroy();
        return false;
    }
    return true;
}

void GBuffer::destroy()
{
    glDeleteFramebuffers(1, &mBufferID);
	for (Texture* tex : mTextures)
	{
        if (tex)
        {
		    tex->unLoad();
		    delete tex;
            tex = nullptr;
        }
	}
}

class Texture* GBuffer::getTexture(Type type)
{
    Texture* tex = nullptr;
    if (this->mTextures.size() > 0)
    {
        tex = this->mTextures[type]; 
    }
    else
    {
        SDL_Log("[GBuffer] Texture is null...");
    }   
    return tex;
}

unsigned int GBuffer::getBufferID() const
{
    return this->mBufferID;
}

void GBuffer::setTexturesActive()
{
    for (int i = 0; i < NUM_GBUFFER_TEXTURES; ++i)
    {
        this->mTextures[i]->setActive(i);
    }
}