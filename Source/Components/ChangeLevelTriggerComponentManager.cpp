#include "stdafx.h"
#include "ChangeLevelTriggerComponentManager.h"
#include "ChangeLevelTriggerComponent.h"
#include "ComponentManager.h"

CChangeLevelTriggerComponentManager* CChangeLevelTriggerComponentManager::ourInstance = nullptr;

void CChangeLevelTriggerComponentManager::Create()
{
	if (ourInstance == nullptr)
		ourInstance = new CChangeLevelTriggerComponentManager();
}

void CChangeLevelTriggerComponentManager::Destroy()
{
	SAFE_DELETE(ourInstance);
}

CChangeLevelTriggerComponentManager & CChangeLevelTriggerComponentManager::GetInstance()
{
	return *ourInstance;
}

ChangeLevelTriggerComponent * CChangeLevelTriggerComponentManager::CreateAndRegisterComponent(const unsigned char aLevelToGoTo, const unsigned short aPortalID)
{													//meybe don't need to send portal ID, check with others.
	ChangeLevelTriggerComponent* component = new ChangeLevelTriggerComponent(aLevelToGoTo, aPortalID);
	CComponentManager::GetInstance().RegisterComponent(component);
	myComponents.Add(component);
	return component;
}

CChangeLevelTriggerComponentManager::CChangeLevelTriggerComponentManager()
{
	myComponents.Init(8);
}

CChangeLevelTriggerComponentManager::~CChangeLevelTriggerComponentManager()
{
}

void CChangeLevelTriggerComponentManager::Update()
{
	// Sets lights dynamically.
	for (int i = 0; i < myComponents.Size(); ++i)
	{
		if (myComponents[i]->GetParent() != nullptr)
		{
			if (myComponents[i]->CheckIfActive() == true)
				myComponents[i]->GetParent()->NotifyComponents(eComponentMessageType::eTurnOnThePointLight, SComponentMessageData());
			else
				myComponents[i]->GetParent()->NotifyComponents(eComponentMessageType::eTurnOffThePointLight, SComponentMessageData());
		}

	}


	// Update Particles

}
