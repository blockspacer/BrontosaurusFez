#include "stdafx.h"
#include "CollisionComponentManager.h"
#include "CollisionComponent.h"

#include "../Collision/CollisionManager.h"

#include "../Collision/PointCollider.h"
#include "../Collision/SquareCollider.h"
#include "../Collision/CircleCollider.h"
#include "../Collision/GroupCollider.h"

CCollisionComponentManager::CCollisionComponentManager()
	: myCollisionComponents(32)
	, myCollisionManager(nullptr)
{
	myCollisionManager = new CCollisionManager();
}


CCollisionComponentManager::~CCollisionComponentManager()
{
	myCollisionComponents.DeleteAll();
	SAFE_DELETE(myCollisionManager);
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

	myCollisionComponents.Add(newCollisionComponent);

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
	ICollider* newCollider = nullptr;

	switch (aColliderType)
	{
	case CCollisionComponentManager::eColliderType::eCircle:
		newCollider = new CCircleCollider();
		break;
	case CCollisionComponentManager::eColliderType::ePoint:
		newCollider = new CPointCollider();
		break;
	case CCollisionComponentManager::eColliderType::eSquare:
		newCollider = new CSquareCollider();
		break;
	}

	return newCollider;
}
