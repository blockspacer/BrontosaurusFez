#pragma once

#include "Component.h"
#include "../CommonUtilities/Stack.h"

#define COMPMGR CComponentManager::GetInstance() 

class CComponentManager
{
public:
	static CComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	ComponentId RegisterComponent(CComponent* aComponent);
	CComponent* GetComponent(ComponentId anId);
	void RemoveComponent(ComponentId anId);

private:
	CComponentManager();
	~CComponentManager();

	static CComponentManager* ourInstance;

	CU::GrowingArray<CComponent*, ComponentId> myComponents;
	CU::Stack<ComponentId, ComponentId> myEmptySpaces;
};

