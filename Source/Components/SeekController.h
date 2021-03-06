#pragma once
#include "IController.h"


class CSeekController : public IController 
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
	void CalledUponForHelp();
	CU::Vector2f& CalculateFormationPosition(const CU::Vector2f& aTargetPositon);

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	inline short GetFormationIndex();
	inline void SetFormationindex(short aIndex);
	inline bool IsActivated();
private:
	CU::Vector2f myVelocity;
	CU::Vector2f myTarget;
	CU::Vector2f myAcceleration;
	CU::Vector2f myFormationDirection;
	CU::Vector2f myStartPosition;

	float myMaxSpeed;
	float myMaxAcceleration;
	float myTargetRadius;
	float mySlowdownRadius;
	float myAggroRange;
	float myCallForHelpRadius;
	float myEvadeRadius;
	float myElapsedEvadeTime;
	float myWaitUntilTurnCountDown;

	unsigned short myFleeingFromDizGuyIndex;
	short myFormationIndex;
	char myShouldAvoidRight;
	char myPrevoiusShouldAvoidRight;
	bool myHaveBeenCalledForHelp;
	bool myShouldGoBackToStatPosition;
	// Inherited via IController
	void Destroy() override;
};

inline short CSeekController::GetFormationIndex()
{
	return myFormationIndex;
}
inline void CSeekController::SetFormationindex(short aIndex)
{
	myFormationIndex = aIndex;
}

inline bool CSeekController::IsActivated()
{
	return myHaveBeenCalledForHelp;
}
