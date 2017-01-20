#include "stdafx.h"
#include "RespawnComponentManager.h"
#include "RespawnComponent.h"
#include "ComponentManager.h"

RespawnComponentManager* RespawnComponentManager::ourInstance = nullptr;

void RespawnComponentManager::Create()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new RespawnComponentManager;
	}
}

void RespawnComponentManager::Destroy()
{
	SAFE_DELETE(ourInstance);
}

RespawnComponentManager & RespawnComponentManager::GetInstance()
{
	return *ourInstance;
}

RespawnComponent * RespawnComponentManager::CreateAndRegisterComponent()
{
	RespawnComponent* component = new RespawnComponent;

	CComponentManager::GetInstance().RegisterComponent(component);

	myComponents.Add(component);

	return component;
}

void RespawnComponentManager::Update(const CU::Time& aDeltaTime)
{
	for (unsigned short i = 0; i < myComponents.Size(); i++)
	{
		myComponents[i]->Update(aDeltaTime.GetSeconds());
	}
}

RespawnComponentManager::RespawnComponentManager()
{
	myComponents.Init(1);
	
}


RespawnComponentManager::~RespawnComponentManager()
{
}
