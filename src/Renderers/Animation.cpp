#include "../../include/Renderers/Animation.h"
#include "../../include/Renderers/Skeleton.h"

#include <document.h>
#include <SDL.h>

Animation::Animation(/* args */)
{
    this->mDuration = 0.0f;
    this->mFrameDuration = 0.0f;
    this->mNumBones = 0;
    this->mNumFrames = 0;
}

Animation::~Animation()
{

}

bool Animation::load(const std::string& fileName)
{
    std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Animation %s", fileName.c_str());
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
		SDL_Log("Animation %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the metadata
	if (ver != 1)
	{
		SDL_Log("Animation %s unknown format", fileName.c_str());
		return false;
	}

	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
	{
		SDL_Log("Animation %s doesn't have a sequence.", fileName.c_str());
		return false;
	}

	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];

	if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint())
	{
		SDL_Log("Sequence %s has invalid frames, length, or bone count.", fileName.c_str());
		return false;
	}

	mNumFrames = frames.GetUint();
	mDuration = length.GetDouble();
	mNumBones = bonecount.GetUint();
	mFrameDuration = mDuration / (mNumFrames - 1);

	this->mTracks.resize(mNumBones);

	const rapidjson::Value& tracks = sequence["tracks"];

	if (!tracks.IsArray())
	{
		SDL_Log("Sequence %s missing a tracks array.", fileName.c_str());
		return false;
	}

	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		if (!tracks[i].IsObject())
		{
			SDL_Log("Animation %s: Track element %d is invalid.", fileName.c_str(), i);
			return false;
		}

		size_t boneIndex = tracks[i]["bone"].GetUint();

		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if (!transforms.IsArray())
		{
			SDL_Log("Animation %s: Track element %d is missing transforms.", fileName.c_str(), i);
			return false;
		}

		BoneTransform temp;

		if (transforms.Size() < mNumFrames)
		{
			SDL_Log("Animation %s: Track element %d has fewer frames than expected.", fileName.c_str(), i);
			return false;
		}

		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++)
		{
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];

			if (!rot.IsArray() || !trans.IsArray())
			{
				SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
				return false;
			}

			temp.mRotation.x = rot[0].GetDouble();
			temp.mRotation.y = rot[1].GetDouble();
			temp.mRotation.z = rot[2].GetDouble();
			temp.mRotation.w = rot[3].GetDouble();

			temp.mTranslation.x = trans[0].GetDouble();
			temp.mTranslation.y = trans[1].GetDouble();
			temp.mTranslation.z = trans[2].GetDouble();

			mTracks[boneIndex].emplace_back(temp);
		}
	}

	return true;
}

size_t Animation::getNumBones() const
{
    return this->mNumBones;
}

size_t Animation::getNumFrames() const
{
    return this->mNumFrames;
}

float Animation::getDuration() const
{
    return this->mDuration;
}

float Animation::getFrameDuration() const
{
    return this->mFrameDuration;
}

void Animation::getGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const
{
    if (outPoses.size() != this->mNumBones)
    {
        outPoses.reserve(this->mNumBones);
    }

	// SDL_Log("Track size: %d", this->mTracks.size());

    // 找到当前帧和下一帧的索引
    size_t frame = static_cast<size_t>(inTime / this->mFrameDuration);
    size_t nextFrame = frame + 1;
    float pct = inTime / this->mFrameDuration - frame;

	// SDL_Log("frame: %d pct: %f, tracks_size: %d, Bone num: %d", frame, pct, this->mTracks.size(), this->mNumBones);

    // 判断骨骼是否包含轨道
    if (this->mTracks[0].size() > 0)
    {
        // 计算根骨骼的位姿
        // 插值
        BoneTransform interp = BoneTransform::interpolate(this->mTracks[0][frame], this->mTracks[0][nextFrame], pct);
        outPoses[0] = interp.toMatrix();

		// SDL_Log("[Animation] Interpolate in root bone");
    }
    else
    {
		// SDL_Log("[Animation] no in root bone");
        outPoses[0] = Matrix4::Identity;
    }

    const std::vector<Skeleton::Bone>& bones = inSkeleton->getBones();
    for (size_t bone = 1; bone < this->mNumBones; ++bone)
    {
        Matrix4 localMat;

        // 判断骨骼是否包含轨道
        if (this->mTracks[bone].size() > 0)
        {
            // 插值
            BoneTransform interp = BoneTransform::interpolate(this->mTracks[bone][frame], this->mTracks[bone][nextFrame], pct);
            localMat = interp.toMatrix();
        }
        outPoses[bone] = localMat * outPoses[bones[bone].mParent];
    }
}