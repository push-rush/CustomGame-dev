#pragma once

#include "./CameraComponent.h"

class MirrorCamera : public CameraComponent
{
public:
	MirrorCamera(class Actor* owner);
    ~MirrorCamera();

	void update(float dt) override;
	void snapToIdeal();

	void setHorzDist(float dist);
	void setVertDist(float dist);
	void setTargetDist(float dist);

	EComponentType getType() const override;

	void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;

private:
	Vector3 computeCameraPos() const;

	float mHorizonDist;
	float mVerticalDist;
	float mTargetDist;
};