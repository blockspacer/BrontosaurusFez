#pragma once
#include "Component.h"
class MovementComponent : public CComponent
{
public:
	MovementComponent();
	~MovementComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	float myMovementSpeed;
	CU::GrowingArray<CU::Vector2f>* myPathPointer;
	unsigned short myCurrentPathIndex;
};

