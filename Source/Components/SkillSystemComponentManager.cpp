#include "stdafx.h"
#include "SkillSystemComponentManager.h"
#include "SkillSystemComponent.h"
#include "ComponentManager.h"

SkillSystemComponentManager* SkillSystemComponentManager::ourInstance = nullptr;

SkillSystemComponentManager::SkillSystemComponentManager()
{
	myComponents.Init(100);
}


SkillSystemComponentManager::~SkillSystemComponentManager()
{
	//for (unsigned int i = 0; i < myComponents.Size(); ++i)
	//{
	//	CComponentManager::GetInstance().RemoveComponent(myComponents[i]->GetId());
	//	SAFE_DELETE(myComponents[i]);
	//}

	myComponents.RemoveAll();
}

SkillSystemComponentManager & SkillSystemComponentManager::GetInstance()
{
	assert(ourInstance != nullptr && "input controller component manager not created (is NULL)");

	return *ourInstance;
}

void SkillSystemComponentManager::CreateInstance()
{
	assert(ourInstance == nullptr && "input controller component manager already created");
	ourInstance = new SkillSystemComponentManager();
}

void SkillSystemComponentManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "input controller component manager not created (is NULL)");

	SAFE_DELETE(ourInstance);
}

void SkillSystemComponentManager::RegisterComponent(SkillSystemComponent * aComponent)
{
	myComponents.Add(aComponent);
}

void SkillSystemComponentManager::Update(const CU::Time & aDeltaTime)
{
	for (unsigned short i = 0; i < myComponents.Size(); i++)
	{
		if (myComponents[i] != nullptr)
		{
			myComponents[i]->Update(aDeltaTime.GetSeconds());
		}
	}
}

void SkillSystemComponentManager::SetGameObjectManager(CGameObjectManager * aGameObjectMan)
{
	myGameObjectManager = aGameObjectMan;
}

void SkillSystemComponentManager::SetCollisionComponentManager(CCollisionComponentManager * aCollisionComponentManager)
{
	myCollisionComponentManager = aCollisionComponentManager;
}

CGameObjectManager * SkillSystemComponentManager::GetGameObjectManager()
{
	return myGameObjectManager;
}

CCollisionComponentManager * SkillSystemComponentManager::GetCollisionComponentManager()
{
	return myCollisionComponentManager;
}

