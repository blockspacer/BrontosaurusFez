#include "stdafx.h"
#include "FleeControllerManager.h"
#include "FleeController.h"
#include "ComponentManager.h"

FleeControllerManager* FleeControllerManager::ourInstance = nullptr;

void FleeControllerManager::Create()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new FleeControllerManager;
	}
}

FleeControllerManager & FleeControllerManager::GetInstance()
{
	return *ourInstance;
}

void FleeControllerManager::Destroy()
{
	delete ourInstance;
	ourInstance = nullptr;
}

CFleeController * FleeControllerManager::CreateAndRegisterController()
{
	CFleeController* flee = new CFleeController();
	CComponentManager::GetInstance().RegisterComponent(flee);
	myControllers.Add(flee);
	return flee;
}

FleeControllerManager::FleeControllerManager()
{
	myControllers.Init(10);
}


FleeControllerManager::~FleeControllerManager()
{
}
