#include "stdafx.h"
#include "HealthRestoreTriggerComponentManager.h"
#include "HealthRestoreTriggerComponent.h"
#include "ComponentManager.h"


void CHealthRestoreTriggerComponentManager::Destroy()
{
}

void CHealthRestoreTriggerComponentManager::Create()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CHealthRestoreTriggerComponentManager;
	}
}

CHealthRestoreTriggerComponentManager & CHealthRestoreTriggerComponentManager::GetInstance()
{
	return *ourInstance;
}

HealthRestoreTriggerComponent * CHealthRestoreTriggerComponentManager::CreateAndRegisterComponent()
{
	HealthRestoreTriggerComponent* component = new HealthRestoreTriggerComponent();
	CComponentManager::GetInstance().RegisterComponent(component);
	myComponents.Add(component);
	return component;
}

CHealthRestoreTriggerComponentManager::CHealthRestoreTriggerComponentManager()
{
	myComponents.Init(10);
}


CHealthRestoreTriggerComponentManager::~CHealthRestoreTriggerComponentManager()
{
}
