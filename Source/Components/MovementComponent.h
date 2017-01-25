#pragma once
#include "Component.h"
#include "../CommonUtilities/vector3.h"

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
	float myMovementSpeed;
	CU::GrowingArray<CU::Vector3f, unsigned int, false>* myPathPointer;
	float myWaitUntilMoveAgianTimer;
	unsigned short myCurrentPathIndex;
	
};

