#include "stdafx.h"
#include "ManaComponentManager.h"
#include"ComponentManager.h"
#include "ManaComponent.h"

ManaComponentManager* ManaComponentManager::ourInstance = nullptr;

ManaComponentManager::ManaComponentManager()
{
	myComponents.Init(100);
}


ManaComponentManager::~ManaComponentManager()
{
}

ManaComponentManager & ManaComponentManager::GetInstance()
{
	assert(ourInstance != nullptr && "SkillComponentManager not created (is NULL)");

	return *ourInstance;
}

void ManaComponentManager::CreateInstance()
{
	assert(ourInstance == nullptr && "SkillComponentManager already created");
	ourInstance = new ManaComponentManager();
}

void ManaComponentManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "ManaComponentManager not created (is NULL)");
	ourInstance->myComponents.Destroy();
	SAFE_DELETE(ourInstance);
}

ManaComponent* ManaComponentManager::CreateAndRegisterComponent(ManaPoint aAmountOfMana)
{
	ManaComponent* Component = new ManaComponent(aAmountOfMana);
	myComponents.Add(Component);
	CComponentManager::GetInstance().RegisterComponent(Component);

	return Component;
}