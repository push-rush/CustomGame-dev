#include "../../include/Game.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Components/SkeletalMeshComponent.h"
#include "../../include/Renderers/VertexArray.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Mesh.h"
#include "../../include/Renderers/Skeleton.h"
#include "../../include/Renderers/Animation.h"
#include "../../include/General/LevelLoader.h"

SkeletalMeshComponent::SkeletalMeshComponent(class Actor* owner)
: MeshComponent(owner, true)
{
    this->mAnimation = nullptr;
	this->mSkeleton = nullptr;
	this->mAnimPlayRate = 0.0f;
	this->mAnimTime = 0.0f;
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{

}

void SkeletalMeshComponent::draw(class Shader* shader)
{
    if (this->getMesh())
	{
		// 设置世界变换矩阵
		shader->setMatrixUniform("uWorldTransform", this->getActor()->getWorldTransform());
		
        // 设置矩阵调色板
		shader->setMatrixUniforms("uMatrixPalette", &this->mPalette.mEntry[0], MAX_SKELETON_BONES);
		
        // Set specular power
		shader->setFloatUniform("uSpecPower", this->getMesh()->getSpecPower());

		// 激活纹理
		Texture* t = this->getMesh()->getTexture(this->getTextureID());
		if (t)
		{
			t->setActive();
		}
		else
		{
			SDL_Log("[SkeletalMeshComponent] Texture is null...");
		}

		// Set the mesh's vertex array as active
		VertexArray* va = this->getMesh()->getVertexArray();
		if (va)
		{
			va->setActive();
		}
		else
		{
			SDL_Log("[SkeletalMeshComponent] Vertex array is null...");
		}
		// Draw
		glDrawElements(
            GL_TRIANGLES, 
            va->getNumIndices(), 
            GL_UNSIGNED_INT, 
            nullptr
        );
	}
	else
	{
		SDL_Log("[SkeletalMeshComponent] Mesh is null...");
	}
}

void SkeletalMeshComponent::computeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = this->mSkeleton->getGlobalInvBindPoses();
	std::vector<Matrix4> currentPoses;
	this->mAnimation->getGlobalPoseAtTime(currentPoses, this->mSkeleton, this->mAnimTime);

	// 为每一个骨骼设置调色板条目
	for (size_t i = 0; i < this->mSkeleton->getBonesNum(); ++i)
	{
		this->mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}

	// SDL_Log("Bone num: %d, Anim time: %f ", this->mSkeleton->getBonesNum(), this->mAnimTime);
}

float SkeletalMeshComponent::playAnimation(class Animation* anim, float playRate)
{
	this->mAnimation = anim;
	this->mAnimTime = 0.0f;
	this->mAnimPlayRate = playRate;

	if (!mAnimation)
	{
		SDL_Log("[SkeletalMeshComponent] Animation is null...");
		return 0.0f;
	}
	this->computeMatrixPalette();
	
	return this->mAnimation->getDuration();
}

void SkeletalMeshComponent::update(float dt)
{
	if (this->mAnimation && this->mSkeleton)
	{
		this->mAnimTime += this->mAnimPlayRate * dt;

		// 判断动画时间是否结束，超出则重置
		while (this->mAnimTime > this->mAnimation->getDuration())
		{
			this->mAnimTime -= this->mAnimation->getDuration();
		}

		// 重新计算矩阵调色板
		this->computeMatrixPalette();
	}
	else
	{
		SDL_Log("[SkeletalMeshComp] Animation or Skeleton is null...");
		if (!this->mAnimation)
		{
			SDL_Log("[SkeletalMeshComp] Animation is null...");
		}

		if (!this->mSkeleton)
		{
			SDL_Log("[SkeletalMeshComp] Skeleton is null...");
		}
	}
}

void SkeletalMeshComponent::setSkeleton(class Skeleton* sk)
{
	this->mSkeleton = sk;
}	

Component::EComponentType SkeletalMeshComponent::getType() const
{
    return ESkeletalMeshComponent;
}

void SkeletalMeshComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

}

void SkeletalMeshComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
    
}