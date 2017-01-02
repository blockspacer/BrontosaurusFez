#pragma once
#include "IController.h"
#include "../CommonUtilities/vector2.h"


class CAIControllerComponent;



class CChaserController : public IController
{
public:
	CChaserController();
	~CChaserController();
	const CU::Vector2f Update(const CU::Time & aDeltaTime) override;
	void SetWeight(const float aWeight);
	void SetTarget(const CU::Vector2f& aPosition);
	void SetMaxSpeed(const float aMaxSpeed);
	void SetMaxAcceleration(const float aMaxAcceleration);
	void SetTargetRadius(const float aTargetRadius);
	void SetSlowDownRadius(const float aSlowdownRadius);
private:
	CU::Vector2f myVelocity;
	CU::Vector2f myTarget;
	CU::Vector2f myAcceleration;

	float myMaxSpeed;
	float myMaxAcceleration;
	float myTargetRadius;
	float mySlowdownRadius;

	// Inherited via IController
	void Destroy() override;
};

