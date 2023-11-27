#include "../../include/Components/MeshComponent.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Mesh.h"
#include "../../include/Renderers/VertexArray.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/General/LevelLoader.h"

#include <glew.h>
#include <glfw3.h>

MeshComponent::MeshComponent(class Actor* owner, bool isSkeletal)
: Component(owner), mIsSkeletal(isSkeletal)
{
    this->mMesh = nullptr;
    this->mTextureIndex = 0;
    this->getActor()->getGame()->getRenderer()->addMeshComponent(this);
}

MeshComponent::~MeshComponent()
{
    this->getActor()->getGame()->getRenderer()->removeMeshComponent(this);
}

void MeshComponent::draw(class Shader* shader)
{
    if (this->mMesh)
    {
        // 设置世界变换矩阵
        shader->setMatrixUniform("uWorldTransform", this->getActor()->getWorldTransform());
        
        // 设置镜面反射率
        shader->setFloatUniform("uSpecPower", this->mMesh->getSpecPower());

        // 激活纹理
        Texture* tex = this->mMesh->getTexture(this->mTextureIndex);
        if (tex)
        {
            tex->setActive();
        }

        // 激活网格顶点数组
        VertexArray* va = this->mMesh->getVertexArray();
        va->setActive();

        glDrawElements(
            GL_TRIANGLES,
            va->getNumIndices(),
            GL_UNSIGNED_INT,
            nullptr
        );  
    }
    else
    {
        // SDL_Log("No mesh...");
    }
}

void MeshComponent::setMesh(class Mesh* mesh)
{
    this->mMesh = mesh;
}

void MeshComponent::setTextureIndex(size_t index)
{
    this->mTextureIndex = index;
}

bool MeshComponent::getIsSkeletal() const
{
    return this->mIsSkeletal;
}

class Mesh* MeshComponent::getMesh() const
{
    return this->mMesh;
}

size_t MeshComponent::getTextureID() const
{
    return this->mTextureIndex;
}


Component::EComponentType MeshComponent::getType() const
{
    return EMeshComponent;
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

    std::string path;
    if (JsonHelper::getString(inObj, "meshFile", path))
    {
        SDL_Log("[MeshComponent] Set mesh component...");
        this->setMesh(this->getActor()->getGame()->getRenderer()->getMesh(path.c_str()));
    }
}

void MeshComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);

    if (this->mMesh)
    {   // 保存网格文件路径
        JsonHelper::addString(alloc, props, "meshFile", this->mMesh->getFileName());
    }
}   