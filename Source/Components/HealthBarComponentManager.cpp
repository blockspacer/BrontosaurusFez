#include "stdafx.h"
#include "HealthBarComponentManager.h"

CHealthBarComponentManager::CHealthBarComponentManager()
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
	return myHealthbars.GetLast();
}

void CHealthBarComponentManager::Update()
{
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
