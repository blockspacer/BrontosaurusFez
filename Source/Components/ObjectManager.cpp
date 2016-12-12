#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "ComponentManager.h"

CGameObjectManager::CGameObjectManager()
{
	InitGrowingArrays();
}

void CGameObjectManager::InitGrowingArrays()
{
	myMatrices.Init(128);
	myObjectsAboutToBeDestroyed.Init(2);
	myObjectsCreated.Init(1024);
}

CGameObjectManager::~CGameObjectManager()
{
	ClearAll();
}


CGameObject* CGameObjectManager::CreateGameObject(const CU::Matrix44f& aMatrix)
{
	CGameObject* object;
	object = new CGameObject(*this);

	CComponentManager::GetInstance().RegisterComponent(object);
	
	myObjectsCreated.Add(object);
	
	object->myTransformId = CreateMatrix(aMatrix);

	return  object;
}

CU::Matrix44f& CGameObjectManager::GetTransform(CGameObject& aGameObject)
{
	return myMatrices[aGameObject.myTransformId];
}

void CGameObjectManager::DestroyObject(CGameObject* aGameObject)
{
	if (aGameObject == nullptr)
	{
		DL_ASSERT("trying to destroy a nullponter as an gameobject");
	}

	CComponentManager::GetInstance().RemoveComponent(aGameObject->GetId());

	myMatrices[aGameObject->myTransformId] = CU::Matrix44f::Identity;
	myFreeMatrices.Push(aGameObject->myTransformId);

	myObjectsCreated.RemoveCyclic(aGameObject);

	delete aGameObject;
}

void CGameObjectManager::AddObjectForDestruction(CGameObject* aGameObject)
{
	myObjectsAboutToBeDestroyed.Add(aGameObject);
}

void CGameObjectManager::DestroyObjectsWaitingForDestruction()
{
	for (unsigned int i = 0; i < myObjectsAboutToBeDestroyed.Size(); ++i)
	{
		myObjectsAboutToBeDestroyed[i]->Destroy();
	}
	myObjectsAboutToBeDestroyed.RemoveAll();
}

void CGameObjectManager::ClearAll()
{
	for (unsigned int i = 0; i < myObjectsCreated.Size(); ++i)
	{
		//DestroyObject(myObjectsCreated[i]);
		SAFE_DELETE(myObjectsCreated[i]);
	}

	myObjectsCreated.RemoveAll();
}

void CGameObjectManager::DumpAllAndReInit()
{
	myMatrices.Destroy();
	myFreeMatrices.Clear();

	//for (unsigned int i = 0; i < myObjectsCreated.Size(); ++i)
	//{
	//	SAFE_DELETE(myObjectsCreated[i]);
	//}
	myObjectsCreated.DeleteAll();

	myObjectsCreated.RemoveAll();
	myObjectsCreated.Destroy();

	myObjectsAboutToBeDestroyed.Destroy();
	
	InitGrowingArrays();
}

ComponentId CGameObjectManager::CreateMatrix(const CU::Matrix44f& aMatrix44f)
{
	if (myFreeMatrices.Size() == 0)
	{
		const ComponentId oldSize = myMatrices.Size();
		myMatrices.Add(aMatrix44f);
		return oldSize;
	}

	const ComponentId freeSlot = myFreeMatrices.Pop();
	myMatrices[freeSlot] = aMatrix44f;

	return freeSlot;
}

