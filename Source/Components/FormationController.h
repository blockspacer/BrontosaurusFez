#pragma once
#include "IController.h"
class FormationController : public IController
{
public:
	FormationController();
	~FormationController();

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
	CU::Vector2f& CalculateFormationPosition(const CU::Vector2f& aTargetPositon);

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	inline short GetFormationIndex();
	inline void SetFormationindex(short aIndex);
	inline bool IsActivated();
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

	short myFormationIndex;
	bool myHaveBeenCalledForHelp;
	// Inherited via IController
	void Destroy() override;
};

inline short FormationController::GetFormationIndex()
{
	return myFormationIndex;
}
inline void FormationController::SetFormationindex(short aIndex)
{
	myFormationIndex = aIndex;
}

inline bool FormationController::IsActivated()
{
	return myHaveBeenCalledForHelp;
}
