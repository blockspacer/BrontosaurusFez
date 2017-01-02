#include "stdafx.h"
#include "CollisionManager.h"
#include "ICollider.h"

#ifndef _RETAIL_BUILD
#define RENDER_DEBUG_LINES
#endif // !_RETAIL_BUILD

CCollisionManager::CCollisionManager()
	: myColliders(32)
	, myShouldRenderDebugLines(false)
{
}

CCollisionManager::~CCollisionManager()
{
	assert(myColliders.Size() > 0 && "Colliders still exist that are not deleted :-(");
}

void CCollisionManager::Update()
{
	for (int i = 0; i < myColliders.Size(); ++i)
	{
		for (int j = i + 1; j < myColliders.Size(); ++j)
		{
			TestCollision(myColliders[i], myColliders[j]);
		}
	}
}

void CCollisionManager::Render()
{
#ifdef RENDER_DEBUG_LINES
	for (ICollider* collider : myColliders)
	{
		collider->RenderDebugLines();
	}
#endif // RENDER_DEBUG_LINES
}

void CCollisionManager::AddCollider(ICollider* aCollider)
{
	if (aCollider == nullptr)
	{
		assert(!"You should not add nullptr to the collision manager ;-)");
		return;
	}

	myColliders.Add(aCollider);
}

void CCollisionManager::RemoveCollider(ICollider* aCollider)
{
	int index = myColliders.Find(aCollider);
	if (index != myColliders.FoundNone)
	{
		myColliders.RemoveCyclicAtIndex(index);
	}
}

void CCollisionManager::TestCollision(ICollider* aFirst, ICollider* aSecond)
{
	if (ICollider::CanCollide(aFirst, aSecond) == false)
	{
		return;
	}

	if (aFirst->TestCollision(aSecond) == true)
	{
		if (ICollider::HasCollided(aFirst, aSecond) == false)
		{
			aFirst->OnCollisionEnter(aSecond);
			aSecond->OnCollisionEnter(aFirst);
		}
		else
		{
			aFirst->OnCollisionUpdate(aSecond);
			aSecond->OnCollisionUpdate(aFirst);
		}
	}
	else
	{
		if (ICollider::HasCollided(aFirst, aSecond) == true)
		{
			aFirst->OnCollisionExit(aSecond);
			aSecond->OnCollisionExit(aFirst);
		}
	}
}
