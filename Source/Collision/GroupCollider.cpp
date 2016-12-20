#include "stdafx.h"
#include "GroupCollider.h"

CGroupCollider::CGroupCollider()
	: myColliders(4ui16)
{
}

CGroupCollider::~CGroupCollider()
{
	myColliders.DeleteAll();
}

void CGroupCollider::RenderDebugLines()
{
	for (ICollider* collider : myColliders)
	{
		collider->RenderDebugLines();
	}
}

bool CGroupCollider::TestCollision(ICollider* aCollider)
{
	return aCollider->TestCollision(this);
}

bool CGroupCollider::TestCollision(CCircleCollider* aCircleCollider)
{
	for (size_type i = 0; i < myColliders.Size(); ++i)
	{
		if (myColliders[i]->TestCollision(aCircleCollider) == true)
		{
			return true;
		}
	}

	return false;
}

bool CGroupCollider::TestCollision(CSquareCollider* aBoxCollider)
{
	for (size_type i = 0; i < myColliders.Size(); ++i)
	{
		if (myColliders[i]->TestCollision(aBoxCollider) == true)
		{
			return true;
		}
	}

	return false;
}

bool CGroupCollider::TestCollision(CGroupCollider* aGroupCollider)
{
	for (size_type i = 0; i < myColliders.Size(); ++i)
	{
		if (myColliders[i]->TestCollision(aGroupCollider) == true)
		{
			return true;
		}
	}

	return false;
}
