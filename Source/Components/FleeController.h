#pragma once
#include "IController.h"

class CGameObject;

class CFleeController : public IController
{
public:
	CFleeController();
	~CFleeController();
	const CU::Vector2f Update(const CU::Time & aDeltaTime) override;

	void SetWeight(const float aWeight);
	void SetFleeRadius(float aRadius);
	void SetTargetsToAvoid( CU::GrowingArray<CGameObject*>* aTargetList);

private:
	void Destroy() override;
	CU::GrowingArray<CGameObject*>* myObjectsToAvoid;
	float myFleeRadius;
};

