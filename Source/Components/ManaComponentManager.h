#pragma once
#include "ManaPoint.h"
class ManaComponent;
class ManaComponentManager
{
public:
	static ManaComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	ManaComponent* CreateAndRegisterComponent(ManaPoint aAmountOfMana);
private:
	ManaComponentManager();
	~ManaComponentManager();

	CU::GrowingArray<ManaComponent*> myComponents;
	static ManaComponentManager* ourInstance;
};

