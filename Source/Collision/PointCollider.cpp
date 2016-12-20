#include "stdafx.h"
#include "PointCollider.h"
#include "CircleCollider.h"

Intersection::SPoint CPointCollider::ourNullPoint = {};

CPointCollider::CPointCollider()
{
	myPointData = new Intersection::SPoint();
}

CPointCollider::CPointCollider(const CU::Vector2f aPosition)
{
	myPointData = new Intersection::SPoint();
	myPointData->myPosition = aPosition;
}

CPointCollider::CPointCollider(const CPointCollider& aCopy)
{
	myPointData = new Intersection::SPoint(*aCopy.myPointData);
}

CPointCollider::~CPointCollider()
{
	SAFE_DELETE(myPointData);
}

bool CPointCollider::TestCollision(ICollider* aCollider)
{
	return aCollider->TestCollision(this);
}

bool CPointCollider::TestCollision(CPointCollider* aPointCollider)
{
	// A point cannot collide with another point due to floating point presision
	// if it is not the same point and then they will (should) not be tested anyway
	return false;
}

bool CPointCollider::TestCollision(CCircleCollider* aCircleCollider)
{
	return Intersection::PointInsideCircle(aCircleCollider->GetData(), *myPointData);
	return false;
}

bool CPointCollider::TestCollision(CSquareCollider* aBoxCollider)
{
	assert(!"point collider not implemented");
	return false;
}

bool CPointCollider::TestCollision(CGroupCollider* aGroupCollider)
{
	assert(!"point collider not implemented");
	return false;
}

const Intersection::SPoint & CPointCollider::GetData() const
{
	if (myPointData == nullptr)
	{
		assert(!"Point data in point collider is NULL");
		return ourNullPoint;
	}

	return *myPointData;
}
