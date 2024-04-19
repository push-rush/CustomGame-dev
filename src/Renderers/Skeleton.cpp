#include "../../include/Renderers/Skeleton.h"
#include "../../include/Components/SkeletalMeshComponent.h"

#include <SDL2/SDL.h>
#include <document.h>

Matrix4 BoneTransform::toMatrix() const
{
    Matrix4 rot = Matrix4::CreateFromQuaternion(this->mRotation);
    Matrix4 trans = Matrix4::CreateTranslation(this->mTranslation);

    return rot * trans;
}

BoneTransform BoneTransform::interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
    BoneTransform retVal;
    retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
    retVal.mTranslation = Vector3::Lerp(a.mTranslation, b.mTranslation, f);

    return retVal;
}

Skeleton::Skeleton()
{

}

Skeleton::~Skeleton()
{

}

bool Skeleton::load(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        SDL_Log("File is not found: Skeleton %s", fileName.c_str());
        return false;
    }

    std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Skeleton %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the metadata
	if (ver != 1)
	{
		SDL_Log("Skeleton %s unknown format", fileName.c_str());
		return false;
	}

	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
	{
		SDL_Log("Skeleton %s doesn't have a bone count.", fileName.c_str());
		return false;
	}

	size_t count = bonecount.GetUint();

	if (count > MAX_SKELETON_BONES)
	{
		SDL_Log("Skeleton %s exceeds maximum bone count.", fileName.c_str());
		return false;
	}

	mBones.reserve(count);

	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray())
	{
		SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
		return false;
	}

	if (bones.Size() != count)
	{
		SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones", fileName.c_str());
		return false;
	}

	Bone temp;
	for (rapidjson::SizeType i = 0; i < count; i++)
	{
		if (!bones[i].IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& name = bones[i]["name"];
		temp.mName = name.GetString();

		const rapidjson::Value& parent = bones[i]["parent"];
		temp.mParent = parent.GetInt();

		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if (!bindpose.IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& rot = bindpose["rot"];
		const rapidjson::Value& trans = bindpose["trans"];

		if (!rot.IsArray() || !trans.IsArray())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		temp.mLocalBindPose.mRotation.x = rot[0].GetDouble();
		temp.mLocalBindPose.mRotation.y = rot[1].GetDouble();
		temp.mLocalBindPose.mRotation.z = rot[2].GetDouble();
		temp.mLocalBindPose.mRotation.w = rot[3].GetDouble();

		temp.mLocalBindPose.mTranslation.x = trans[0].GetDouble();
		temp.mLocalBindPose.mTranslation.y = trans[1].GetDouble();
		temp.mLocalBindPose.mTranslation.z = trans[2].GetDouble();

		this->mBones.emplace_back(temp);
	}

	// 计算骨骼全局反向绑定姿势矩阵
	this->computeGlobalInvBindPose();

	return true;
}

size_t Skeleton::getBonesNum() const
{
    return this->mBones.size();
}

const Skeleton::Bone& Skeleton::getBone(size_t idx) const
{
    return this->mBones[idx];
}

const std::vector<Skeleton::Bone>& Skeleton::getBones() const
{
    return this->mBones;
}

const std::vector<Matrix4>& Skeleton::getGlobalInvBindPoses() const
{
    return this->mGlobalInvBindPoses;
}

void Skeleton::computeGlobalInvBindPose()
{
    this->mGlobalInvBindPoses.reserve(this->getBonesNum());
	SDL_Log("[Skeleton] Vector capacity: %d and size: %d", this->mGlobalInvBindPoses.capacity(), this->mGlobalInvBindPoses.size());

    // 计算每个骨骼的全局绑定姿势
	Matrix4 globalInvBindPose = this->mBones[0].mLocalBindPose.toMatrix();
    this->mGlobalInvBindPoses.emplace_back(globalInvBindPose);
    for (size_t i = 1; i < this->mGlobalInvBindPoses.capacity(); ++i)
    {
        Matrix4 localMat = this->mBones[i].mLocalBindPose.toMatrix();
        globalInvBindPose = localMat * this->mGlobalInvBindPoses[this->mBones[i].mParent];
		this->mGlobalInvBindPoses.emplace_back(globalInvBindPose);
    }

    for (size_t i = 0; i < this->mGlobalInvBindPoses.size(); ++i)
    {
        this->mGlobalInvBindPoses[i].Invert();
    }
	SDL_Log("GlbalinvBindPose size: %d and capacity: %d", this->mGlobalInvBindPoses.size(), this->mGlobalInvBindPoses.capacity());
}