#pragma once
#include "Component.h"
#include "IController.h"
#include "../CommonUtilities/vector2.h"

//Blended Behaviour Controller
class CAIControllerComponent : public CComponent
{
public:
	CAIControllerComponent();
	~CAIControllerComponent();

	void AddControllerBehaviour(IController* aController);
	void RemoveControllerBehaviour(IController* aController);

	const CU::Vector2f& GetVelocity();
	void Update(const CU::Time& aDeltaTime);
	void Destroy() override;

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;

private:
	CU::GrowingArray<IController*> myControllers;
	CU::Vector2f myVelocity;

	float myMaxVelocity;
};

