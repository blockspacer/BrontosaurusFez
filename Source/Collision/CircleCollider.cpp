#include "stdafx.h"

#include "CircleCollider.h"
#include "Intersection.h"

Intersection::SCircle CCircleCollider::ourNullCircle = {};

CCircleCollider::CCircleCollider()
{
	myCircleData = new Intersection::SCircle();
}

CCircleCollider::CCircleCollider(const CCircleCollider& aCopy)
{
	myCircleData = new Intersection::SCircle(*aCopy.myCircleData);
}

CCircleCollider::~CCircleCollider()
{
	SAFE_DELETE(myCircleData);
}

bool CCircleCollider::TestCollision(ICollider* aCollider)
{
	return aCollider->TestCollision(this);
}

bool CCircleCollider::TestCollision(CPointCollider* aCollider)
{
	assert(!"point collider not implemented");
	return false;
}

bool CCircleCollider::TestCollision(CCircleCollider* aCircleCollider)
{
	if (Intersection::CircleVsCircle(*myCircleData, *aCircleCollider->myCircleData))
	{
		return true;
	}

	return false;
}

bool CCircleCollider::TestCollision(CSquareCollider* aBoxCollider)
{
	assert(!"point collider not implemented");
	return false;
}

bool CCircleCollider::TestCollision(CGroupCollider* aGroupCollider)
{
	assert(!"point collider not implemented");
	return false;
}

const Intersection::SCircle& CCircleCollider::GetData() const
{
	if (myCircleData == nullptr)
	{
		assert(!"Circle data in circle collider is NULL");
		return ourNullCircle;
	}

	return *myCircleData;
}
