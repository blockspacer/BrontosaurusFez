#include "stdafx.h"
#include "AIControllerManager.h"
#include "ComponentManager.h"

AIControllerManager* AIControllerManager::ourInstance = nullptr;

AIControllerManager::AIControllerManager()
{
	myControllers.Init(24);
}


AIControllerManager::~AIControllerManager()
{
	for (unsigned int i = 0; i < myControllers.Size(); ++i)
	{
		CComponentManager::GetInstance().RemoveComponent(myControllers[i]->GetId());
	}
	myControllers.DeleteAll();
}

void AIControllerManager::Create()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new AIControllerManager();
	}
}

void AIControllerManager::Destroy()
{
	if (ourInstance != nullptr)
	{
		delete ourInstance;
		ourInstance = nullptr;
	}
}

AIControllerManager & AIControllerManager::GetInstance()
{
	return *ourInstance;
}

void AIControllerManager::AddController(CAIControllerComponent * aController)
{
	myControllers.Add(aController);
}

void AIControllerManager::Update(const CU::Time & aDeltaTime)
{
	for (unsigned int i = 0; i < myControllers.Size(); ++i)
	{
		myControllers[i]->Update(aDeltaTime);
	}
}