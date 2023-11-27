#include "../../include/Actors/SplineActor.h"
#include "../../include/Components/SplineCamera.h"

SplineActor::SplineActor(class Game* game)
: Actor(game)
{
    this->mCameraComponent = new SplineCamera(this);

    // Create a spline
	Spline splinePath;
    splinePath.mControlPoints.emplace_back(Vector3::Zero);
	for (int i = 0; i < 5; i++)
	{
		if (i % 2 == 0)
		{
			splinePath.mControlPoints.emplace_back(Vector3(300.0f * (i + 1), 300.0f, 300.0f));
		}
		else
		{
			splinePath.mControlPoints.emplace_back(Vector3(300.0f * (i + 1), 0.0f, 0.0f));
		}
	}

    this->mCameraComponent->setPaused(false);
    this->mCameraComponent->setSpline(splinePath);
}

SplineActor::~SplineActor()
{

}

void SplineActor::actorInput(const uint8_t* keyboardStates)
{

}

Actor::EActorType SplineActor::getType() const
{
    return ESplineActor;
}

void SplineActor::loadProperties(const rapidjson::Value& inObj)
{
    Actor::loadProperties(inObj);
}

void SplineActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Actor::saveProperties(alloc, props);
}