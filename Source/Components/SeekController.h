#pragma once
#include "IController.h"


class CSeekController : public IController  // Bugg, fiender kan typ g� in i orbit.
{
public:
	CSeekController();
	~CSeekController();
	const CU::Vector2f Update(const CU::Time & aDeltaTime) override;
	void SetWeight(const float aWeight);
	void SetTarget(const CU::Vector2f& aPosition);
	void SetMaxSpeed(const float aMaxSpeed);
	void SetMaxAcceleration(const float aMaxAcceleration);
	void SetTargetRadius(const float aTargetRadius);
	void SetSlowDownRadius(const float aSlowdownRadius);
	void SetAggroRange(const float aRange);
	void SetCallForHelpRange(const float aRange);
	void CallForHelp();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;

private:
	CU::Vector2f myVelocity;
	CU::Vector2f myTarget;
	CU::Vector2f myAcceleration;

	float myMaxSpeed;
	float myMaxAcceleration;
	float myTargetRadius;
	float mySlowdownRadius;
	float myAggroRange;
	float myCallForHelpRadius;

	bool myHaveBeenCalledForHelp;
	// Inherited via IController
	void Destroy() override;
};

