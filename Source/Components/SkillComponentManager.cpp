#include "stdafx.h"
#include "SkillComponentManager.h"
#include"ComponentManager.h"
#include "SkillComponent.h"

SkillComponentManager::SkillComponentManager()
{
	myComponents.Init(100);
}


SkillComponentManager::~SkillComponentManager()
{
}

SkillComponentManager & SkillComponentManager::GetInstance()
{
	assert(ourInstance != nullptr && "SkillComponentManager not created (is NULL)");

	return *ourInstance;
}

void SkillComponentManager::CreateInstance()
{
	assert(ourInstance == nullptr && "SkillComponentManager already created");
	ourInstance = new SkillComponentManager();
}

void SkillComponentManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "SkillComponentManager not created (is NULL)");
	ourInstance->myComponents.Destroy();
	SAFE_DELETE(ourInstance);
}

SkillComponent* SkillComponentManager::CreateAndRegisterComponent()
{
	SkillComponent* Component;
	myComponents.Add(Component);
	CComponentManager::GetInstance().RegisterComponent(Component);

	return Component;
}

void SkillComponentManager::Update(const CU::Time & aDeltaTime)
{
	for (SkillComponent* component : myComponents)
	{
		if (component != nullptr)
		{
			component->Update(aDeltaTime.GetSeconds());
		}
	}
}
