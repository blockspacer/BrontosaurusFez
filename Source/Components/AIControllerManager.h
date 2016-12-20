#pragma once
#include "IController.h"
#include "AIControllerComponent.h"
class AIControllerManager
{
public:
	static void Create();
	static void Destroy();

	static AIControllerManager& GetIstance();

	void AddController(CAIControllerComponent* aController);
	void Update(const CU::Time& aDeltaTime);
private:
	CU::GrowingArray<CAIControllerComponent*> myControllers;

	static AIControllerManager* ourInstance;

	AIControllerManager();
	~AIControllerManager();
};

