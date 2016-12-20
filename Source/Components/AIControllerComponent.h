#pragma once
#include "Component.h"
#include "IController.h"

class CAIControllerComponent : public CComponent
{
public:
	CAIControllerComponent();
	~CAIControllerComponent();

	void AddControllerBehaviour(IController* aController);
	void RemoveControllerBehaviour(IController* aController);

	void Update(const CU::Time& aDeltaTime);
	void Destroy() override;
private:
	CU::GrowingArray<IController*> myControllers;
	CU::Vector2f myVelocity;

	float myMaxVelocity;
};

