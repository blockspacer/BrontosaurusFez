#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "ComponentManager.h"

CObjectManager::CObjectManager()
{
	InitGrowingArrays();
}

void CObjectManager::InitGrowingArrays()
{
	myMatrices.Init(128);
	myObjectsAboutToBeDestroyed.Init(2);
	myObjectsCreated.Init(1024);
}

CObjectManager::~CObjectManager()
{
	ClearAll();
}


CGameObject* CObjectManager::CreateGameObject(const CU::Matrix44f& aMatrix)
{
	CGameObject* object;
	object = new CGameObject(*this);

	CComponentManager::GetInstance().RegisterComponent(object);
	
	myObjectsCreated.Add(object);
	
	object->myTransformId = CreateMatrix(aMatrix);

	return  object;
}

CU::Matrix44f& CObjectManager::GetTransform(CGameObject& aGameObject)
{
	return myMatrices[aGameObject.myTransformId];
}

void CObjectManager::DestroyObject(CGameObject* aGameObject)
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

void CObjectManager::AddObjectForDestruction(CGameObject* aGameObject)
{
	myObjectsAboutToBeDestroyed.Add(aGameObject);
}

void CObjectManager::DestroyObjectsWaitingForDestruction()
{
	for (unsigned int i = 0; i < myObjectsAboutToBeDestroyed.Size(); ++i)
	{
		myObjectsAboutToBeDestroyed[i]->Destroy();
	}
	myObjectsAboutToBeDestroyed.RemoveAll();
}

void CObjectManager::ClearAll()
{
	for (unsigned int i = 0; i < myObjectsCreated.Size(); ++i)
	{
		//DestroyObject(myObjectsCreated[i]);
		SAFE_DELETE(myObjectsCreated[i]);
	}

	myObjectsCreated.RemoveAll();
}

void CObjectManager::DumpAllAndReInit()
{
	myMatrices.Destroy();
	myFreeMatrices.Clear();

	for (unsigned int i = 0; i < myObjectsCreated.Size(); ++i)
	{
		SAFE_DELETE(myObjectsCreated[i]);
	}

	myObjectsCreated.RemoveAll();
	myObjectsCreated.Destroy();

	myObjectsAboutToBeDestroyed.Destroy();
	
	InitGrowingArrays();
}

ComponentId CObjectManager::CreateMatrix(const CU::Matrix44f& aMatrix44f)
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

