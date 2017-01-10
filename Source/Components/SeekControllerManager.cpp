#include "stdafx.h"
#include "SeekControllerManager.h"
#include "SeekController.h"
#include "ComponentManager.h"
#include "../Game/PollingStation.h"

CSeekControllerManager* CSeekControllerManager::ourInstance = nullptr;

void CSeekControllerManager::Create()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CSeekControllerManager();
	}
}

CSeekControllerManager & CSeekControllerManager::GetInstance()
{
	return *ourInstance;
}

void CSeekControllerManager::Destroy()
{
	delete ourInstance;
	ourInstance = nullptr;
}

void CSeekControllerManager::SetTarget()
{
	for (unsigned short i = 0; i < myControllers.Size(); i++)
	{
		myControllers[i]->SetTarget(PollingStation::playerObject->GetWorldPosition());
	}
}

CSeekController * CSeekControllerManager::CreateAndRegister()
{
	CSeekController* seek = new CSeekController();
	CComponentManager::GetInstance().RegisterComponent(seek);
	myControllers.Add(seek);

	return seek;
}

CSeekControllerManager::CSeekControllerManager()
{
	myControllers.Init(10);
}


CSeekControllerManager::~CSeekControllerManager()
{
}
