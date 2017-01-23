#include "stdafx.h"
#include "ComponentManager.h"

CComponentManager* CComponentManager::ourInstance = nullptr;

CComponentManager::CComponentManager()
{
	myComponents.Init(128);
	myComponents.Add(nullptr);
}


CComponentManager::~CComponentManager()
{
}

CComponentManager& CComponentManager::GetInstance()
{
	assert(ourInstance != nullptr && "component manager not created (is NULL)");
	return *ourInstance;
}

void CComponentManager::CreateInstance()
{
	assert(ourInstance == nullptr && "component manager already created");
	ourInstance = new CComponentManager();
}

void CComponentManager::DestroyInstance()
{
	//DESTROY EVERYTHING.
	for (int i = 0; i < ourInstance->myComponents.Size(); ++i)
	{
		if (ourInstance->myComponents[i] != nullptr)
		{
			ourInstance->RemoveComponent(ourInstance->myComponents[i]->GetId());
		}
	}


	assert(ourInstance != nullptr && "component manager not created (is NULL)");
	SAFE_DELETE(ourInstance);
}

ComponentId CComponentManager::RegisterComponent(CComponent* aComponent)
{
	if (myEmptySpaces.Size() == 0)
	{
		myComponents.Add(aComponent);
		aComponent->myId = myComponents.Size() - 1;
		return myComponents.Size() - 1;
	}
	else
	{
		myComponents[myEmptySpaces.Top()] = aComponent;
		aComponent->myId = myEmptySpaces.Top();
		return myEmptySpaces.Pop();
	}
}

CComponent* CComponentManager::GetComponent(ComponentId anId)
{
	if (anId == NULL_COMPONENT)
	{
		DL_ASSERT("trying to get the null compmonent");
	}

	if (myComponents[anId] == nullptr)
	{
		DL_PRINT_WARNING("Warning trying to use a removed component at %d may get access violation", anId);
	}

	return myComponents[anId];
}

void CComponentManager::RemoveComponent(ComponentId anId)
{
	delete myComponents[anId];
	myComponents[anId] = nullptr;
	myEmptySpaces.Push(anId);
}
