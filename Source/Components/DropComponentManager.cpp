#include "stdafx.h"
#include "DropComponentManager.h"
#include"ComponentManager.h"
#include "DropComponent.h"

DropComponentManager* DropComponentManager::ourInstance = nullptr;

DropComponentManager::DropComponentManager()
{
	myComponents.Init(100);
}


DropComponentManager::~DropComponentManager()
{
}

DropComponentManager & DropComponentManager::GetInstance()
{
	assert(ourInstance != nullptr && "SkillComponentManager not created (is NULL)");

	return *ourInstance;
}

void DropComponentManager::CreateInstance()
{
	assert(ourInstance == nullptr && "SkillComponentManager already created");
	ourInstance = new DropComponentManager();
}

void DropComponentManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "SkillComponentManager not created (is NULL)");
	ourInstance->myComponents.Destroy();
	SAFE_DELETE(ourInstance);
}

DropComponent* DropComponentManager::CreateAndRegisterComponent()
{
	DropComponent* Component;
	myComponents.Add(Component);
	CComponentManager::GetInstance().RegisterComponent(Component);

	return Component;
}

void DropComponentManager::Update(const CU::Time & aDeltaTime)
{
	for (DropComponent* component : myComponents)
	{
		if (component != nullptr)
		{
			component->Update(aDeltaTime.GetSeconds());
		}
	}
}
