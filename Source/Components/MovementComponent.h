#pragma once
#include "Component.h"
#include "../CommonUtilities/vector3.h"

class CPath;
struct SNavmeshTriangle;

class MovementComponent : public CComponent
{
public:
	MovementComponent();
	~MovementComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

	void SetMovementSpeed(const float aMovementspeed);
private:
	bool IsOnNavmesh(const CU::Vector3f& aPosition);


private:
	CPath* myPathPointer;
	SNavmeshTriangle* myTriangle;
	float myMovementSpeed;
	float myWaitUntilMoveAgianTimer;
	unsigned short myCurrentPathIndex;
	bool myIsActive;
	bool myIsFollowingPath;
};

