#include "stdafx.h"
#include "HealthBarComponentManager.h"
#include "Camera.h"
#include "ComponentManager.h"

CHealthBarComponentManager::CHealthBarComponentManager(const CU::Camera& aCamera) : myPlayerCamera(aCamera)
{
	myHealthbars.Init(64);
}

CHealthBarComponentManager::~CHealthBarComponentManager()
{
}

CHealthBarComponent* CHealthBarComponentManager::GetHealthBarAt(short aIndex)
{
	return myHealthbars.At(aIndex);
}

CHealthBarComponent* CHealthBarComponentManager::CreateHealthbar()
{
	myHealthbars.Add(new CHealthBarComponent());
	CComponentManager::GetInstance().RegisterComponent(myHealthbars.GetLast());
	myHealthbars.GetLast()->SetCamera(myPlayerCamera);
	return myHealthbars.GetLast();
}

void CHealthBarComponentManager::Update()
{
	for (int i = 0; i < myHealthbars.Size(); ++i)
	{
		if (myHealthbars[i]->ShouldBeDeleted() == true)
		{
			myHealthbars.RemoveCyclicAtIndex(i);
		}
	}

	for (int i = 0; i < myHealthbars.Size(); ++i)
	{
		myHealthbars[i]->Update();
	}
}

void CHealthBarComponentManager::Render()
{
	for (int i = 0; i < myHealthbars.Size(); ++i)
	{
		myHealthbars[i]->Render();
	}
}
