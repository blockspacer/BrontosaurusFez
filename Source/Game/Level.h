#pragma once
//#include "GrowingArray.h"
#include "..\Components/ObjectManager.h"
#include "..\Components\Component.h"

class CGameObject;

class CLevel
{
public:
	CLevel();
	~CLevel();

	CGameObject* CreateObject(const CU::Matrix44f &aTransform);
	void AddObject(CGameObject* aGameObject);

private:
	CU::GrowingArray<CGameObject*> myGameObjects;
	CGameObjectManager myObjectManager;
};

