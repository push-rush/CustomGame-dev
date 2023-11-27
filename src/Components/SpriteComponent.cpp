#include "../../include/Components/SpriteComponent.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/General/LevelLoader.h"

SpriteComponent::SpriteComponent(/* args */)
{

}

SpriteComponent::SpriteComponent(class Actor* owner, int updateOrder)
: Component(owner)
{
    mDrawOrder = updateOrder;
    mTexture = nullptr;
    mTexWidth = 0;
    mTexHeight = 0;

    this->getActor()->getGame()->getRenderer()->addSpriteComponent(this);
}

SpriteComponent::~SpriteComponent()
{
    this->getActor()->removeComponent(this);
}

void SpriteComponent::draw(class Shader* shader)
{
    shader->setActive();
    if (this->mTexture)
    {
        Matrix4 scaleMat = Matrix4::CreateScale(
            (float)this->mTexWidth,
            (float)this->mTexHeight,
            1.0f
        );
        Matrix4 worldTransform = scaleMat * this->getActor()->getWorldTransform();
        shader->setMatrixUniform("uWorldTransform", worldTransform);

        this->mTexture->setActive();
        // 绘制三角形
        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            nullptr
        );
    }
}

void SpriteComponent::setTexture(Texture* texture)
{
    if (texture)
    {
        this->mTexture = texture;
        this->mTexWidth = texture->getWidth();
        this->mTexHeight = texture->getHeight();
        // SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
    }
    else
    {
        SDL_Log("[SpriteComponent] Texture is null...");
    }
}

int SpriteComponent::getDrawOrder() const
{
    return this->mDrawOrder;
}

int SpriteComponent::getTexHeight() const
{
    return this->mTexHeight;
}

int SpriteComponent::getTexWidth() const
{
    return this->mTexWidth;
}

Component::EComponentType SpriteComponent::getType() const
{
    return ESpriteComponent;
}

void SpriteComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

    std::string path;
    if (JsonHelper::getString(inObj, "textureFile", path))
    {
        SDL_Log("[SpriteComponent] Set sprite component...");
        this->setTexture(this->getActor()->getGame()->getRenderer()->getTexture(path.c_str()));
    }
}

void SpriteComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);

    if (this->mTexture)
    {   // 保存网格文件路径
        JsonHelper::addString(alloc, props, "textureFile", this->mTexture->getFileName());
    }
}