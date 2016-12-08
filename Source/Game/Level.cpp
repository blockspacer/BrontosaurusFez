#include "stdafx.h"
#include "Level.h"
#include "..\Components/GameObject.h"


CLevel::CLevel()
{
	myGameObjects.Init(32);
}


CLevel::~CLevel()
{
}

CGameObject* CLevel::CreateObject(const CU::Matrix44f& aTransform)
{
	return myObjectManager.CreateGameObject(aTransform);
}

void CLevel::AddObject(CGameObject* aGameObject)
{
	if (aGameObject != nullptr)
	{
		myGameObjects.Add(aGameObject);
	}
}
