#include "stdafx.h"
#include "BlessingTowerComponentManager.h"
#include"ComponentManager.h"
#include "BlessingTowerComponent.h"

BlessingTowerComponentManager* BlessingTowerComponentManager::ourInstance = nullptr;

BlessingTowerComponentManager::BlessingTowerComponentManager()
{
	myComponents.Init(100);
}


BlessingTowerComponentManager::~BlessingTowerComponentManager()
{
}

BlessingTowerComponentManager & BlessingTowerComponentManager::GetInstance()
{
	assert(ourInstance != nullptr && "BlessingTowerComponentManager not created (is NULL)");

	return *ourInstance;
}

void BlessingTowerComponentManager::CreateInstance()
{
	assert(ourInstance == nullptr && "BlessingTowerComponentManager already created");
	ourInstance = new BlessingTowerComponentManager();
}

void BlessingTowerComponentManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "BlessingTowerComponentManager not created (is NULL)");
	ourInstance->myComponents.Destroy();
	SAFE_DELETE(ourInstance);
}

BlessingTowerComponent* BlessingTowerComponentManager::CreateAndRegisterComponent()
{
	BlessingTowerComponent* Component = new BlessingTowerComponent();
	myComponents.Add(Component);
	CComponentManager::GetInstance().RegisterComponent(Component);

	return Component;
}

void BlessingTowerComponentManager::Update(const CU::Time & aDeltaTime)
{
	for (unsigned short i = 0; i < myComponents.Size(); i++)
	{
		if (myComponents[i] != nullptr)
		{
			myComponents[i]->Update(aDeltaTime.GetSeconds());
		}
	}
}