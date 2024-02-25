#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <time.h>

// sdl
#include <SDL.h>
#include <SDL_image.h>

// opengl
#include <glew.h>
#include <glfw3.h>

#include "./General/Cmath.h"

struct DirectionalLight
{
	// 光线方向
	Vector3 mDirection;
	
    // 漫反射颜色
	Vector3 mDiffuseColor;
	
    // 镜面反射颜色
	Vector3 mSpecColor;
};

class Renderer
{
private:
    SDL_Window* mWindow;
    SDL_GLContext mGLContext;
    std::unordered_map<std::string, class Texture*> mTextures;

    // 视图投影矩阵
    class Matrix4 mView;
    class Matrix4 mProjection;

    // 屏幕宽高
    float mScreenWidth;
    float mScreenHeight;

    // ui字体
    class Shader* mFontShader;

    // 精灵类型
    class Shader* mSpriteShader;
    class VertexArray* mSpriteVerts;
    std::vector<class SpriteComponent*> mSprites;

    // 基础颜色着色器
    class Shader* mBasicShader;
    
    // 网格类型
    class Shader*  mMeshShader;
    std::vector<class MeshComponent*> mMeshComponents;
    std::unordered_map<std::string, class Mesh*> mMeshes;
    
    // 骨骼类型
    class Shader* mSkeletalShader;
    std::vector<class SkeletalMeshComponent*> mSkeletalComponents;

    // 灯光数据
    Vector3 mAmbientLight;
    DirectionalLight mDirLight;

    int mDir;
    float mColorCount;
    Uint32 mTicksCount;

    // 帧缓冲区对象
    unsigned int mMirrorBuffer;
    class Texture* mMirrorTexture;
    Matrix4 mMirrorView;

    // G缓冲区对象
    class GBuffer* mGBuffer;
    class Shader* mGGlobalShader;

    // ui缓存区对象
    // unsigned int mUIBuffer;
    // class Texture* mUITexture;

    // 点光源
    std::vector<class PointLightComponent*> mPointLightComps;
    class Shader* mGPointLightShader;
    class Mesh* mPointLightMesh;

    class Game* mGame;
    
public:
    Renderer(class Game* game);
    ~Renderer();

    void draw();
    void draw(class Texture* tex, const Vector2& viewportScale = Vector2{1.0f, 1.0f}, 
        const Vector2& offset = Vector2{0.0f, 0.0f}, const Vector2& trans = Vector2{0.0f, 0.0f}
    );

    bool loadShaders();
    void createSpriteVerts();
    void unLoadData();
    
    // getters
    class Mesh* getMesh(const char* fileName);
    class Texture* getTexture(const char* fileName); 
    class Game* getGame();
    class Shader* getSpriteShader() const;
    float getScreenHeight() const;
    float getScreenWidth() const;
    class Texture* getMirrorTexture() const;
    class GBuffer* getGBuffer() const;
    const Vector3& getAmbientLight() const;
    class Shader* getFontShader() const;
    class SDL_Window* getRenderWindow() const;
    class VertexArray* getSpriteVerts() const;
    class Shader* getBasicShader() const;

    // class Texture* getUITexture() const;
    // unsigned int getUIBuffer() const;

    // setters
    void setViewMatrix(Matrix4 view);
    void setAmbientLight(Vector3);
    void setLightUniforms(class Shader* shader, const Matrix4& view);
    void setMirrorView(Matrix4 view);
    
    // 添加精灵组件
    void addSpriteComponent(class SpriteComponent* sprite);
    void removeSpriteComponent(class SpriteComponent* sprite);
    
    // 添加网格组件
    void addMeshComponent(class MeshComponent* mc);
    void removeMeshComponent(class MeshComponent* mc);

    // 添加点光源组件
    void addPointLightComponent(class PointLightComponent* light);
	void removePointLightComponent(class PointLightComponent* light);
    
    DirectionalLight& getDirectionalLight();
    bool init(int x = 150, int y = 75, int width = 1024, int height = 568);

    // 逆投影
    Vector3 unProject(const Vector3& screenPoint) const;

    // 计算屏幕某点的空间方向向量
    Vector3 getScreenDirection(const Vector2& point2d) const;
    void getScreenDirection(Vector3& start, Vector3& outDir);

    // 创建配置后视镜帧缓冲区
    bool createMirrorTarget();

    // 创建配置ui帧缓存区
    bool createUIFrameBuffer(unsigned int& buffer, class Texture*& tex, bool enable_depth = false);

    // 绘制3D场景
    void draw3DScene(unsigned int frameBuffer, const Matrix4& view, const Matrix4& proj, 
                        float viewportScale = 1.0f, bool limit = true);

    // 绘制G缓冲区四边形
    void drawFromGBuffer(const unsigned& buffer_id);
    
    // 绘制ui
    bool drawUI(class UIScreen* ui, const unsigned int& frameBuffer, 
        const Vector3& color = Vector3{0.0f, 0.0f, 0.0f}, const float& viewportScale = 1.0f, bool clear_buffer = true
    );
};