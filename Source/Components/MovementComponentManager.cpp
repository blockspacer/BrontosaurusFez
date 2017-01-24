#include "stdafx.h"
#include "MovementComponentManager.h"
#include "MovementComponent.h"
#include "ComponentManager.h"


MovementComponentManager* MovementComponentManager::ourInstance = nullptr;

MovementComponentManager::MovementComponentManager()
{
	myComponents.Init(2);
}


MovementComponentManager & MovementComponentManager::GetInstance()
{
	assert(ourInstance != nullptr && "enemy ai controller component manager not created (is NULL)");

	return *ourInstance;
}

void MovementComponentManager::CreateInstance()
{
	assert(ourInstance == nullptr && "enemy ai controller component manager already created");
	ourInstance = new MovementComponentManager();
}

void MovementComponentManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "enemy ai controller component manager not created (is NULL)");

	SAFE_DELETE(ourInstance);
}

MovementComponent* MovementComponentManager::CreateAndRegisterComponent()
{
	MovementComponent* Component;
	Component = new MovementComponent();
	myComponents.Add(Component);
	CComponentManager::GetInstance().RegisterComponent(Component);

	return Component;
}

void MovementComponentManager::Update(const CU::Time & aDeltaTime)
{
	for (unsigned short i = 0; i < myComponents.Size(); i++)
	{
		if (myComponents[i] != nullptr)
		{
			myComponents[i]->Update(aDeltaTime.GetSeconds());
		}
	}
}

MovementComponentManager::~MovementComponentManager()
{
	//for (unsigned int i = 0; i < myComponents.Size(); ++i)
	//{
	//	CComponentManager::GetInstance().RemoveComponent(myComponents[i]->GetId());
	//	SAFE_DELETE(myComponents[i]);
	//}

	myComponents.RemoveAll();
}
