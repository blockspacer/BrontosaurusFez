#include "stdafx.h"
#include "InventoryComponentManager.h"
#include "InventoryComponent.h"
#include "ComponentManager.h"
#include <GrowingArray.h>

CInventoryComponentManager* CInventoryComponentManager::ourInstance = nullptr;

CInventoryComponentManager::CInventoryComponentManager()
{
	myComponents.Init(8);
}

CInventoryComponentManager& CInventoryComponentManager::GetInstance()
{
	if (ourInstance == nullptr)
		ourInstance = new CInventoryComponentManager();
	return *ourInstance;
}

void CInventoryComponentManager::Update(const CU::Time & aDeltaTime)
{
	 // updatera shizz mebe
	// Kanske inte behöver en manager;
}

CInventoryComponent& CInventoryComponentManager::CreateAndRegisterComponent()
{
	CInventoryComponent* component = new CInventoryComponent();
	myComponents.Add(component);
	COMPMGR.RegisterComponent(component);
	return *component;
}

void CInventoryComponentManager::DestroyInstance()
{
	SAFE_DELETE(ourInstance);
}

CInventoryComponentManager::~CInventoryComponentManager()
{
	myComponents.DeleteAll();
}