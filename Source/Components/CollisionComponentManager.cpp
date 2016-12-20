#include "stdafx.h"
#include "CollisionComponentManager.h"
#include "CollisionComponent.h"

#include "../Collision/CollisionManager.h"


CCollisionComponentManager::CCollisionComponentManager()
	: myCollisionComponents(32)
	, myCollisionManager(nullptr)
{
	myCollisionManager = new CCollisionManager();
}


CCollisionComponentManager::~CCollisionComponentManager()
{
	myCollisionComponents.DeleteAll();
}

void CCollisionComponentManager::Update()
{
	myCollisionManager->Update();
	myCollisionManager->Render();
}

CCollisionComponent* CCollisionComponentManager::CreateCollisionComponent(const eColliderType aColliderType)
{
	ICollider* newCollider = CreateCollider(aColliderType);
	CCollisionComponent* newCollisionComponent = new CCollisionComponent(newCollider);



	return newCollisionComponent;
}

void CCollisionComponentManager::DestroyCollisionComponent(CCollisionComponent* aCollisionComponent)
{
	unsigned int index = myCollisionComponents.Find(aCollisionComponent);
	if (index != myCollisionComponents.FoundNone)
	{
		myCollisionComponents.DeleteCyclicAtIndex(index);
	}
}

ICollider* CCollisionComponentManager::CreateCollider(const eColliderType aColliderType)
{
	return nullptr;
}
