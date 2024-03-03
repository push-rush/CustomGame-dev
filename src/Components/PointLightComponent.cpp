#include "../../include/Components/PointLightComponent.h"

#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/Mesh.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/VertexArray.h"

#include "../../include/Actors/Actor.h"

#include "../../include/General/LevelLoader.h"
#include "../../include/General/ResourceManager.h"

#include "../../include/Game.h"

PointLightComponent::PointLightComponent(class Actor* owner)
: Component(owner)
{
     ResourceManager::ResourceProperty* rep = new ResourceManager::ResourceProperty{
        this,
        "PointLight." + to_string(this->getCompID()),
        ResourceManager::ELight,
        ResourceManager::EUnactivited
    };
    this->getActor()->getGame()->getResourceManager()->addResourceProperty(rep);

    owner->getGame()->getRenderer()->addPointLightComponent(this);
}

PointLightComponent::~PointLightComponent()
{
    this->getActor()->getGame()->getRenderer()->removePointLightComponent(this);
}

void PointLightComponent::draw(class Shader* lightShader, class Mesh* lightMesh)
{
    Matrix4 scale = Matrix4::CreateScale(this->getActor()->getScale() * this->mOuterRadius / lightMesh->getRadius());
    Matrix4 trans = Matrix4::CreateTranslation(this->getActor()->getPosition());
    Matrix4 worldTransform = scale * trans;
    lightShader->setMatrixUniform("uWorldTransform", worldTransform);
    // lightShader->setFloatUniform("uSpecPower", lightMesh->getSpecPower());

    // 设置点光源着色器常量
    lightShader->setVectorUniform("uPointLight.mWorldPos", this->getActor()->getPosition());
    lightShader->setVectorUniform("uPointLight.mDiffuseColor", this->mDiffuseColor);
    lightShader->setVectorUniform("uPointLight.mSpecColor", this->mSpecColor);
    lightShader->setFloatUniform("uPointLight.mInnerRadius", this->mInnerRadius);
    lightShader->setFloatUniform("uPointLight.mOuterRadius", this->mOuterRadius);

    // 绘制球体
    glDrawElements(
        GL_TRIANGLES,
        lightMesh->getVertexArray()->getNumIndices(),
        GL_UNSIGNED_INT, 
        nullptr
    );
}

Component::EComponentType PointLightComponent::getType() const
{
    return EPointLightComponent;
}

void PointLightComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

    JsonHelper::getVector3(inObj, "diffuseColor", this->mDiffuseColor);
    JsonHelper::getVector3(inObj, "specColor", this->mSpecColor);
    JsonHelper::getScale(inObj, "innerRadius", this->mInnerRadius);
    JsonHelper::getScale(inObj, "outerRadius", this->mOuterRadius);
}

void PointLightComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
    JsonHelper::addVector3(alloc, props, "diffuseColor", this->mDiffuseColor);
    JsonHelper::addVector3(alloc, props, "specColor", this->mSpecColor);
    JsonHelper::addScale(alloc, props, "innerRadius", this->mInnerRadius);
    JsonHelper::addScale(alloc, props, "outerRadius", this->mOuterRadius);
}