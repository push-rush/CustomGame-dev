#include "../../include/Renderers/Renderer.h"

#include "../../include/Renderers/VertexArray.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Components/SpriteComponent.h"
#include "../../include/Renderers/VertexArray.h"
#include "../../include/Renderers/UIScreen.h"
#include "../../include/Renderers/GBuffer.h"
#include "../../include/Renderers/Mesh.h"
#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/HUD.h"
#include "../../include/Renderers/Graphics3d.h"

// #include "../../include/Collision.h"
// #include "../../include/MeshComponent.h"
#include "../../include/Components/SkeletalMeshComponent.h"
#include "../../include/Components/PointLightComponent.h"

#include "../../include/Actors/FPSActor.h"

#include "../../include/Game.h"

Renderer::Renderer(Game* game)
{
    this->mGame = game;
    this->mWindow = nullptr;
    this->mMeshShader = nullptr;
    this->mSpriteShader = nullptr;
    this->mMirrorTexture = nullptr;

    this->mDir = 1;
    this->mColorCount = 0;
    this->mTicksCount = 0;

    // this->mUITexture = nullptr;
}

Renderer::~Renderer()
{
    // 删除OpenGL上下文
    SDL_GL_DeleteContext(this->mGLContext);

    // SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
}

void Renderer::createSpriteVerts()
{
    float vertexBuffer[] = {
        -0.5f,  0.5f, 0.0f, 0.f, 0.f, 0.f,  0.f,  0.f,
         0.5f,  0.5f, 0.0f, 0.f, 0.f, 0.f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.f, 0.f, 0.f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.f, 0.f, 0.f, 0.0f, 1.0f
    };

    uint32_t indexBuffer[] = {
        0, 1, 2,
        2, 3, 0
    };

    this->mSpriteVerts = new VertexArray(VertexArray::PosNormTex, vertexBuffer, 4, indexBuffer, 6);
}

Texture* Renderer::getTexture(const char* fileName)
{
    Texture* tex = nullptr;

    auto iter = this->mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        tex = new Texture();
        if (tex->load(fileName))
        {
            this->mTextures.emplace(fileName, tex);
        }
        else
        {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

bool Renderer::loadShaders()
{
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(this->mScreenWidth, this->mScreenHeight);

    /********* 设置基础颜色着色器 *********/
    this->mBasicShader = new Shader();
    if (!this->mBasicShader->Load("../Shaders/Basic.vert", "../Shaders/Basic.frag"))
    {
        SDL_Log("[Renderer] Load basic shader failed...");
        return false;
    }
    this->mBasicShader->setActive();
    this->mBasicShader->setMatrixUniform("uViewProj", viewProj);
    this->mBasicShader->setVectorUniform("uColor", Vector3{0.8f, 0.35f, 0.35f});

    /********* 设置ui字体着色器 *********/
    this->mFontShader = new Shader();
    if (!this->mFontShader->Load("../Shaders/Font.vert", "../Shaders/Font.frag"))
    {
        SDL_Log("[Renderer] Load font shader failed...");
        return false;
    }
    this->mFontShader->setActive();
    this->mFontShader->setVectorUniform("uColor", Vector3{0.8f, 0.35f, 0.35f});
    this->mFontShader->setMatrixUniform("uViewProj", viewProj);

    /********** 设置精灵着色器 ***********/
    this->mSpriteShader = new Shader();
    if (!this->mSpriteShader->Load("../Shaders/Sprite.vert", "../Shaders/Sprite.frag"))
    {
		SDL_Log("[Renderer] Load sprite shader failed...");
        return false;
    }
    this->mSpriteShader->setActive();
    this->mSpriteShader->setMatrixUniform("uViewProj", viewProj);

    /********** 设置网格着色器 ***********/
    this->mMeshShader = new Shader();
    if (!this->mMeshShader->Load("../Shaders/Phong.vert", "../Shaders/GBufferWrite.frag"))
    {
		SDL_Log("[Renderer] Load mesh shader failed...");
        return false;
    }

    this->mMeshShader->setActive();
    // 设置视图矩阵
    this->mView = Matrix4::CreateLookAt(
        Vector3::Zero,   // 相机位置
        Vector3::UnitX,  // 目标位置
        Vector3::UnitZ   // 上 
    );

    // 设置投影矩阵
    this->mProjection = Matrix4::CreatePerspectiveFOV(
        Math::ToRadians(70.0f), // 水平视场角
        this->mScreenWidth,     // 视野宽度
        this->mScreenHeight,    // 视野高度
        10.0f,                  //近平面距离
        10000.0                 //远平面距离
    );
    this->mMeshShader->setMatrixUniform("uViewProj", this->mView * this->mProjection);

    /**************** 基础网格着色器 ******************/
    this->mBasicMeshShader = new Shader();
    if (!this->mBasicMeshShader->Load("../Shaders/BasicMesh.vert", "../Shaders/BasicMesh.frag"))
    {
		SDL_Log("[Renderer] Load mesh shader failed...");
        return false;
    }
    this->mBasicMeshShader->setActive();
    this->mBasicMeshShader->setMatrixUniform("uViewProj", this->mView * this->mProjection);

    /********** 设置蒙皮着色器 ***********/
    this->mSkeletalShader = new Shader();
    if (!this->mSkeletalShader->Load("../Shaders/Skinned.vert", "../Shaders/GBufferWrite.frag"))
    {
        SDL_Log("[Renderer] Load Skeletal shader failed...");
        return false;
    }

    this->mSkeletalShader->setActive();
    // this->mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    // this->mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), this->mScreenWidth, this->mScreenHeight, 10.0f, 10000.0f);
    this->mSkeletalShader->setMatrixUniform("uViewProj", this->mView * this->mProjection);

    /********** 设置GBuffer全局光照着色器 **************/
    this->mGGlobalShader = new Shader();
    if (!this->mGGlobalShader->Load("../Shaders/GBufferGlobal.vert", "../Shaders/GBufferGlobal.frag"))
    {
        SDL_Log("[Renderer] Load GBuffer shader failed...");
        return false;
    }
    this->mGGlobalShader->setActive();
    this->mGGlobalShader->setIntUniform("uGDiffuse", 0);
    this->mGGlobalShader->setIntUniform("uGNormal", 1);
    this->mGGlobalShader->setIntUniform("uGWorldPos", 2);
    this->mGGlobalShader->setIntUniform("uGSpecPower", 3);

    this->mGGlobalShader->setMatrixUniform("uViewProj", viewProj);
    Matrix4 gbufferWorld = Matrix4::CreateScale(this->mScreenWidth, -this->mScreenHeight, 1.0f);
    this->mGGlobalShader->setMatrixUniform("uWorldTransform", gbufferWorld);

    /********** 设置点光源着色器 ***********/
    this->mGPointLightShader = new Shader();
    if (!this->mGPointLightShader->Load("../Shaders/BasicMesh.vert", "../Shaders/GBufferPointLight.frag"))
    {
        SDL_Log("[Renderer] Load PointLight shader failed...");
        return false;
    }
    this->mGPointLightShader->setActive();
    this->mGPointLightShader->setIntUniform("uGDiffuse", 0);
    this->mGPointLightShader->setIntUniform("uGNormal", 1);
    this->mGPointLightShader->setIntUniform("uGWorldPos", 2);
    this->mGPointLightShader->setIntUniform("uGSpecPower", 3);
    this->mGPointLightShader->setVector2Uniform("uScreenDimensions", Vector2(this->mScreenWidth, this->mScreenHeight));
    
    return true;
}

Mesh* Renderer::getMesh(const char* fileName)
{
    Mesh* mesh = nullptr;
    auto iter = this->mMeshes.find(fileName);
    if (iter != this->mMeshes.end())
    {
        mesh = (*iter).second;
    }
    else
    {
        mesh = new Mesh();
        if (mesh->load(fileName, this))
        {
            this->mMeshes.emplace(fileName, mesh);
        }
        else
        {
            delete mesh;
            mesh = nullptr;
        }
    }
    return mesh;
}

void Renderer::addSpriteComponent(class SpriteComponent* sprite)
{
    int updateOrder = sprite->getUpdateOrder();
    auto iter = this->mSprites.begin();
    for (; iter != this->mSprites.end(); ++iter)
    {
        if (updateOrder < (*iter)->getUpdateOrder())
        {
            SDL_Log("添加一个精灵对象...");
            break;
        }
    }

    this->mSprites.insert(iter, sprite);
}

void Renderer::removeSpriteComponent(SpriteComponent* sprite)
{
    for (auto iter = mSprites.begin(); iter != mSprites.end(); ++iter)
    {
        if ((*iter) != nullptr)
        {
            if ((*iter) == sprite)
            {
                iter = mSprites.erase(iter);
                break;
            }
        }
    }

    if (sprite)
    {
        delete sprite;
        sprite = nullptr;
    }
}

void Renderer::draw()
{
    this->draw3DScene(this->mMirrorBuffer, this->mMirrorView, this->mProjection, 0.20f);
    this->draw3DScene(this->mGBuffer->getBufferID(), this->mView, this->mProjection, 1.0f, false);

    this->mBasicMeshShader->setActive();
    this->mBasicMeshShader->setMatrixUniform("uViewProj", this->mView * mProjection);
    
    // 设置世界变换矩阵
    // this->mMeshShader->setMatrixUniform("uWorldTransform", this->getGame()->getPlayer()->getWorldTransform());
    
    // 设置镜面反射率
    // this->mMeshShader->setFloatUniform("uSpecPower", 0.2f);

    auto hud = this->getGame()->getHUD();

    // auto click_center3d = hud->getClickPoint();
    // Renderers::Graphics3d::drawCylinder(this->mMeshShader, click_center3d, 25.0f, 100.0f, Vector3{0.75f, 0.5f, 0.25f}, 4);
    // Renderers::Graphics3d::drawCube(this->mBasicMeshShader, click_center3d, 60.0f, 50.0f, 50.0f, Vector3{0.85f, 0.55f, 0.85f}, true);

    glBindFramebuffer(GL_FRAMEBUFFER, hud->getBindFrameBuffer());
    glClearColor(hud->getUIBGColor().x, hud->getUIBGColor().y, hud->getUIBGColor().z, 1.0f);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glBindFramebuffer(GL_FRAMEBUFFER, hud->getBindFrameBuffer());
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // 从G缓存区绘制
    this->drawFromGBuffer(hud->getBindFrameBuffer());

    // 绘制HUD元素
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // this->drawUI();
    for (auto ui : this->getGame()->getUIStack())
    {
        if ((ui->getUIState() == UIScreen::EActive || ui->getUIState() == UIScreen::EVisible))
        {
            if (ui->getUIType() == UIScreen::EHUD)
                this->drawUI(ui, ui->getBindFrameBuffer(), ui->getUIBGColor(), 1.0f, false);
            else
                this->drawUI(ui, ui->getBindFrameBuffer(), ui->getUIBGColor());

            this->draw(ui->getBindTexture(), ui->getUIViewScale(), ui->getUIPosOffset());
        }
    }

    // 绘制菜单ui元素
    // this->drawUI(this->mUIBuffer, 1.0f);

    // 前后台缓存区互换
    SDL_GL_SwapWindow(mWindow);
}

void Renderer::draw(class Texture* tex, const Vector2& viewportScale, const Vector2& offset, const Vector2& trans)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLint w = static_cast<int>(this->mScreenWidth * viewportScale.x);
    GLint h = static_cast<int>(this->mScreenHeight * viewportScale.y);
    glViewport(offset.x + (mScreenWidth - w) * 0.5, offset.y + (mScreenHeight - h) * 0.5, w, h);

    this->mSpriteShader->setActive();
    this->mSpriteVerts->setActive();

    // 判断是否需要翻转y坐标
    Matrix4 scaleMat = Matrix4::CreateScale(((float)tex->getWidth() * (1.0f / viewportScale.x)), -(float)tex->getHeight() * (1.0f / viewportScale.y), 1.0f);
    Matrix4 transMat = Matrix4::CreateTranslation(Vector3{(trans.x - offset.x) * (1.0f / viewportScale.x), (trans.y - offset.y) * (1.0f / viewportScale.y), 0.0f});
    Matrix4 world = scaleMat * transMat;
    
    tex->setActive();
    this->mSpriteShader->setMatrixUniform("uWorldTransform", world);
    // this->mSpriteShader->setMatrixUniform("uViewProj", mView * mProjection);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Renderer::drawUI(class UIScreen* ui, const unsigned int& frameBuffer, const Vector3& color, const float& viewportScale, bool clear_buffer)
{
    // 设置帧缓存区索引
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // 清空颜色缓存区
    if (clear_buffer)
    {
        glClearColor(color.x, color.y, color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // 基于比例设置视窗大小
    GLint w = static_cast<int>(this->mScreenWidth * viewportScale);
    GLint h = static_cast<int>(this->mScreenHeight * viewportScale);
    glViewport((mScreenWidth - w) * 0.5, (mScreenHeight - h) * 0.5, w, h);

    // 禁用深度缓存区
    glDisable(GL_DEPTH_TEST);

    // 开启Alpha混合渲染
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    ui->draw(this->mBasicShader, this->mSpriteShader, this->mFontShader, nullptr);
    return true;
}

// bool Renderer::drawUI(class UIScreen* ui, const unsigned int& frameBuffer, const Vector3& color, const float& viewportScale)
// {
//     // 设置帧缓存区索引为0
//     glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

//     // 基于比例设置视窗大小
//     GLint w = static_cast<int>(this->mScreenWidth * viewportScale);
//     GLint h = static_cast<int>(this->mScreenHeight * viewportScale);
//     glViewport((mScreenWidth - w) * 0.5, (mScreenHeight - h) * 0.5, w, h);

//     // 禁用深度缓存区
//     glDisable(GL_DEPTH_TEST);

//     // 开启Alpha混合渲染
//     glEnable(GL_BLEND);

//     // 清空颜色缓存区
//     float r = float(16.0f / 255.0f);
//     float g = float(135.0f / 255.0f);
//     float b = float(122.0f / 255.0f);
//     float alpha = 1.0f;
//     glClearColor(r, g, b, alpha);
//     glClear(GL_COLOR_BUFFER_BIT);

//     // 绘制ui元素
//     this->mSpriteShader->setActive();
//     this->mSpriteVerts->setActive();
//     // this->mSpriteShader->setMatrixUniform("uViewProj", mView * mProjection);

//     bool paused = false;
//     for (auto ui : this->getGame()->getUIStack())
//     {
//         if (ui->getUIType() != UIScreen::EHUD && ui->getUIType() != UIScreen::EResourceMenu &&
//             (ui->getUIState() == UIScreen::EActive || ui->getUIState() == UIScreen::EVisible)
//         )
//         {
//             // ui->setBGPos(Vector2{128.0f, 70.0f});
//             ui->draw(this->mSpriteShader, this->mFontShader);
//             paused = true;

//             // SDL_Log("[Renderer] Active ui type: %d", ui->getUIType());
//         }
//         // else if (ui->getUIType() == UIScreen::EDialogBox)
//         // {
//         //     SDL_Log("[Renderer] Dialogbox state: %d", ui->getUIState());
//         // }
//     }

//     if (paused)
//     {
//         glBindFramebuffer(GL_FRAMEBUFFER, 0);

//         GLint w = static_cast<int>(this->mScreenWidth * 0.80);
//         GLint h = static_cast<int>(this->mScreenHeight * 0.80);
//         glViewport((mScreenWidth - w) * 0.5, (mScreenHeight - h) * 0.5, w, h);

//         // 禁用深度缓存区
//         glDisable(GL_DEPTH_TEST);
        
//         // 开启Alpha混合渲染
//         glEnable(GL_BLEND);
//         // glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//         // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

//         this->mSpriteShader->setActive();
//         this->mSpriteVerts->setActive();
//         Texture* texture = this->mUITexture;
//         if (texture)
//         {
//             // 判断是否需要翻转y坐标
//             Matrix4 scaleMat = Matrix4::CreateScale(((float)texture->getWidth() * 1.25f), -(float)texture->getHeight() * 1.25f, 1.0f);
//             Matrix4 transMat = Matrix4::CreateTranslation(Vector3(0.0, 0.0, 0.0f));
//             Matrix4 world = scaleMat * transMat;
            
//             texture->setActive();
//             this->mSpriteShader->setMatrixUniform("uWorldTransform", world);
//             // this->mSpriteShader->setMatrixUniform("uViewProj", mView * mProjection);
//             glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//         }
//     }

//     return paused;
// }

// void Renderer::drawUI(float viewportScale)
// {
//     /********* 绘制ui精灵 ***********/

//     // 禁用深度缓存区
//     glDisable(GL_DEPTH_TEST);
//     // 开启Alpha混合渲染
//     glEnable(GL_BLEND);
//     glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//     glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

//     this->mSpriteShader->setActive();
//     this->mSpriteVerts->setActive();
//     for (auto sprite : this->mSprites)
//     {
//         if (sprite != nullptr)
//         {
//             sprite->draw(this->mSpriteShader);
//         }
//     }
//     // SDL_Log("SC size: %d", this->mSprites.size());

//     // 绘制ui元素
//     for (auto ui : this->getGame()->getUIStack())
//     {
//         if ((ui->getUIType() == UIScreen::EHUD || ui->getUIType() == UIScreen::EResourceMenu) &&
//             (ui->getUIState() == UIScreen::EActive || ui->getUIState() == UIScreen::EVisible)
//         )
//         {
//             ui->draw(this->mSpriteShader, this->mFontShader);
//         }
//     }
// }

bool Renderer::init(int x, int y, int width, int height)
{
    this->mScreenWidth = width;
    this->mScreenHeight = height;

    // 设置OpenGL窗口属性
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    // 设置OpenGL版本
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // 设置RGBA颜色通道位宽
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    
    // 使用双缓冲区
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // 申请深度缓存区空间
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // glEnable(GL_DEPTH_TEST);
    
    // 指定使用硬件加速，即渲染在GPU上运行
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // 创建OpenGL窗口
    mWindow = SDL_CreateWindow(
        "Graphics3D",     // 窗口标题
        x,    // 左上角（x，y）坐标
        y,
        width,   // 窗口宽度
        height,    // 窗口高度
        SDL_WINDOW_OPENGL  // 可选的窗口创建标志
    );
    if (!mWindow)
    {
        SDL_Log("Create window failed: %s", SDL_GetError());
        return false;
    }

    // 创建OpenGL上下文
    this->mGLContext = SDL_GL_CreateContext(mWindow);

    // 设置视口
    // glViewport(x, y, width, height);

    // 初始化GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("Failed to init GLEW: %s", SDL_GetError());
        return false;
    }

    // 清楚良性错误代码
    glGetError();

    if (SDL_Init(IMG_INIT_PNG) != 0)
    {
        SDL_Log("Failed to init SDL_Image: %s", SDL_GetError());
    }

    // 加载着色器源文件
    if (!this->loadShaders())
    {   
        SDL_Log("Failed to load shaders.");
        return false;
    }

    // 初始化精灵数组（矩形）
    this->createSpriteVerts();

    // 创建镜面对象
    if (!this->createMirrorTarget())
    {
        SDL_Log("[Renderer] Create mirror target failed...");
        return false;
    }

    // 创建ui缓存区对象
    // if (!this->createUIFrameBuffer(this->mUIBuffer, this->mUITexture))
    // {
    //     SDL_Log("[Renderer] Create ui buffer failed...");
    //     return false;
    // }

    // 创建GBuffer对象
    this->mGBuffer = new GBuffer();
    if (!this->mGBuffer->create(width, height))
    {
        SDL_Log("[Renderer] Failed to create G-Buffer...");
        return false;
    }

    // 加载点光源网格
    this->mPointLightMesh = this->getMesh("../Assets/Meshes/PointLight.gpmesh");

    return true;
}

class Game* Renderer::getGame()
{
    return this->mGame;
}

void Renderer::addMeshComponent(class MeshComponent* mc)
{
    if (mc->getIsSkeletal())
    {
        SkeletalMeshComponent* sc = reinterpret_cast<SkeletalMeshComponent*>(mc);
        this->mSkeletalComponents.emplace_back(sc);
    }
    else
    {
        this->mMeshComponents.emplace_back(mc);
    }
    // SDL_Log("add mc: %d", this->mMeshComponents.size());
}

void Renderer::removeMeshComponent(class MeshComponent* mc)
{
    if (mc->getIsSkeletal())
    {
        SkeletalMeshComponent* sc = reinterpret_cast<SkeletalMeshComponent*>(mc);
        for (auto iter = this->mSkeletalComponents.begin(); iter != this->mSkeletalComponents.end();)
        {
            if ((*iter) == sc)
            {
                iter = this->mSkeletalComponents.erase(iter);
                break;
            }
            else
            {
                ++iter;
            }
        }
    }
    else
    {
        for (auto iter = this->mMeshComponents.begin(); iter != this->mMeshComponents.end();)
        {
            if ((*iter) == mc)
            {
                iter = this->mMeshComponents.erase(iter);
                break;
            }
            else
            {
                ++iter;
            }
        }
    }
}

void Renderer::setViewMatrix(Matrix4 view)
{
    this->mView = view;
}

void Renderer::setAmbientLight(Vector3 ambient)
{
    this->mAmbientLight = ambient;
}

DirectionalLight& Renderer::getDirectionalLight()
{
    return this->mDirLight;
}

void Renderer::setLightUniforms(class Shader* shader, const Matrix4& view)
{
    Matrix4 invView = view;
    invView.Invert();

    shader->setVectorUniform("uCameraPos", invView.GetTranslation());

    shader->setVectorUniform("uAmbientLight", this->mAmbientLight);

    shader->setVectorUniform(
        "uDirLight.mDirection",
        this->mDirLight.mDirection
    );
    shader->setVectorUniform(
        "uDirLight.mDiffuseColor",
        this->mDirLight.mDiffuseColor
    );
    shader->setVectorUniform(
        "uDirLight.mSpecColor",
        this->mDirLight.mSpecColor
    );
}

Vector3 Renderer::unProject(const Vector3& screenPoint) const
{
    Vector3 deviceCoord = screenPoint;
    deviceCoord.x /= (this->mScreenHeight * 0.5);
    deviceCoord.y /= (this->mScreenWidth * 0.5);

    Matrix4 unProjection = this->mView * this->mProjection;
    unProjection.Invert();
    return Vector3::TransformWithPerspDiv(deviceCoord, unProjection);
}

Vector3 Renderer::getScreenDirection(const Vector2& point2d) const
{
    Vector3 screenPointNearPlane = Vector3(point2d.x, point2d.y, 0.0f);
    Vector3 spacePointNearPlane = this->unProject(screenPointNearPlane);
    screenPointNearPlane.z = 0.9f;
    Vector3 spacePointFarPlane = this->unProject(screenPointNearPlane);

    // 方向向量
    Vector3 outDir = (spacePointFarPlane - spacePointNearPlane);
    outDir.Normalize();
    return outDir;
}

void Renderer::getScreenDirection(Vector3& start, Vector3& outDir)
{
    Vector3 screenPointNearPlane = Vector3(0.0f, 0.0f, 0.0f);
    start = this->unProject(screenPointNearPlane);

    screenPointNearPlane.z = 0.9f;
    Vector3 end = this->unProject(screenPointNearPlane);

    // 方向向量
    outDir = end - start;
    outDir.Normalize();
}

class Shader* Renderer::getSpriteShader() const
{
    return this->mSpriteShader;
}

class Shader* Renderer::getFontShader() const
{
    return this->mFontShader;
}

float Renderer::getScreenHeight() const
{
    return this->mScreenHeight;
}

float Renderer::getScreenWidth() const
{
    return this->mScreenWidth;
}

void Renderer::unLoadData()
{
    for (auto tex : this->mTextures)
    {
        tex.second->unLoad();
        delete tex.second;
    }
    this->mTextures.clear();

    for (auto mesh : this->mMeshes)
    {
        mesh.second->unLoad();
        delete mesh.second;
    }
    this->mMeshes.clear();
}

bool Renderer::createMirrorTarget()
{
    int width = static_cast<int>(this->mScreenWidth / 5.f);
    int height = static_cast<int>(this->mScreenHeight / 5.f);

    // 为后视镜纹理生成帧缓存区
    glGenFramebuffers(1, &this->mMirrorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->mMirrorBuffer);

    // 创建渲染的纹理
    this->mMirrorTexture = new Texture();
    this->mMirrorTexture->createForRendering(width, height, GL_RGB);

    // 添加深度缓存区
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    // 将后视镜纹理作为帧缓存区的输出对象
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->mMirrorTexture->getTextureID(), 0);

    // 设置帧缓存区绘制的列表
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    // 检查任务正常执行
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // 如果任务未正常执行，删除帧缓存区
        glDeleteFramebuffers(1, &this->mMirrorBuffer);

        if (this->mMirrorTexture)
        {
            this->mMirrorTexture->unLoad();
            delete this->mMirrorTexture;
            this->mMirrorTexture = nullptr;
        }        
        return false;
    }
    return true;
}

void Renderer::draw3DScene(unsigned int frameBuffer, const Matrix4& view, const Matrix4& proj, float viewportScale, bool limit)
{
    // 设置当前的帧缓存区
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // 基于比例设置视窗大小
    glViewport(0, 0, static_cast<int>(this->mScreenWidth * viewportScale), static_cast<int>(this->mScreenHeight * viewportScale));

    // 清空颜色/深度缓存区
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /************* 绘制3D网格 *************/
    
    // 关闭颜色缓冲区混合
    glDisable(GL_BLEND);
    
    // 打开深度缓冲区
    glEnable(GL_DEPTH_TEST);

    this->mMeshShader->setActive();
    this->mMeshShader->setMatrixUniform("uViewProj", view * proj);

    if (limit)
    {
        this->setLightUniforms(this->mMeshShader, view);
    }
    for (auto mc : this->mMeshComponents)
    {
        if (mc != nullptr)
        {
            mc->draw(this->mMeshShader);
            // SDL_Log("MC size: %d", this->mMeshComponents.size());
        }
    }

    /********* 绘制蒙皮 ***********/

    this->mSkeletalShader->setActive();
    this->mSkeletalShader->setMatrixUniform("uViewProj", view * proj);
    if (limit)
    {
        this->setLightUniforms(this->mSkeletalShader, view);
    }
    for (auto sc : this->mSkeletalComponents)
    {
        if (sc != nullptr)
        {
            sc->draw(this->mSkeletalShader);
        }
        else
        {
            SDL_Log("[Renderer] Skeleton is null...");
        }
    }
}

Texture* Renderer::getMirrorTexture() const
{
    return this->mMirrorTexture;
}

void Renderer::setMirrorView(Matrix4 view)
{
    this->mMirrorView = view;
}

void Renderer::drawFromGBuffer(const unsigned& buffer_id)
{
    glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
    
    glDisable(GL_DEPTH_TEST);

    this->mGGlobalShader->setActive();

    this->mSpriteVerts->setActive();

    this->mGBuffer->setTexturesActive();

    this->setLightUniforms(this->mGGlobalShader, this->mView);

    glDrawElements(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        nullptr
    );

    // /******** 绘制点光源 *******/
    // // 将G缓存区中的深度缓存区绘制到默认帧缓冲区的深度缓存区中
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->mGBuffer->getBufferID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer_id);
    int width = static_cast<int>(this->mScreenWidth);
    int height = static_cast<int>(this->mScreenHeight);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    // 重启深度缓冲区测试，但是禁止深度缓存区写入
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // 激活点光源着色器和网格
    this->mGPointLightShader->setActive();
    this->mPointLightMesh->getVertexArray()->setActive();

    // 设置视图投影矩阵
    this->mGPointLightShader->setMatrixUniform("uViewProj", this->mView * this->mProjection);
    
    // 设置相机位置
    Matrix4 cameraPos = this->mView;
    cameraPos.Invert();
    this->mGPointLightShader->setVectorUniform("uCameraPos", cameraPos.GetTranslation());
    this->mGBuffer->setTexturesActive();

    // 允许点光源颜色混合到已存在的颜色中
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    // 绘制点光源
    for (auto p : this->mPointLightComps)
    {
        p->draw(this->mGPointLightShader, this->mPointLightMesh);
    }

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void Renderer::addPointLightComponent(class PointLightComponent* light)
{
    this->mPointLightComps.emplace_back(light);
}

void Renderer::removePointLightComponent(class PointLightComponent* light)
{
    for (auto iter = this->mPointLightComps.begin(); iter != this->mPointLightComps.end();)
    {
        if ((*iter) == light)
        {
            iter = this->mPointLightComps.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }
}

class GBuffer* Renderer::getGBuffer() const
{
    return this->mGBuffer;
}

const Vector3& Renderer::getAmbientLight() const
{
    return this->mAmbientLight;
}

SDL_Window* Renderer::getRenderWindow() const
{
    return this->mWindow;
}

VertexArray* Renderer::getSpriteVerts() const
{
    return this->mSpriteVerts;
}

bool Renderer::createUIFrameBuffer(unsigned int& buffer, class Texture*& tex, bool enable_depth)
{
    int width = static_cast<int>(this->mScreenWidth * 1.0f);
    int height = static_cast<int>(this->mScreenHeight * 1.0f);

    // 生成帧缓存区
    glGenFramebuffers(1, &buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);

    // 创建渲染的纹理
    if (!tex)
    {
        tex = new Texture();
        tex->createForRendering(width, height, GL_RGB);
    }
    else
    {
        SDL_Log("[Renderer] Texture is not null...");
        return false;
    }

    // 添加深度缓存区
    if (enable_depth)
    {
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    }

    // 将纹理作为帧缓存区的输出对象
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex->getTextureID(), 0);

    // 设置帧缓存区绘制的列表
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    // 检查任务正常执行
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // 如果任务未正常执行，删除帧缓存区
        glDeleteFramebuffers(1, &buffer);

        if (tex)
        {
            tex->unLoad();
            delete tex;
            tex = nullptr;
        }        
        return false;
    }
    return true;
}

// Texture* Renderer::getUITexture() const
// {
//     return this->mUITexture;
// }

// unsigned int Renderer::getUIBuffer() const
// {
//     return this->mUIBuffer;
// }

class Shader* Renderer::getBasicShader() const
{
    return this->mBasicShader;
}

Shader* Renderer::getBasicMeshShader() const
{
    return this->mBasicMeshShader;
}