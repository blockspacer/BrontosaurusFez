#include "stdafx.h"

#include "CircleCollider.h"
#include "Intersection.h"

const Intersection::SCircle CCircleCollider::ourNullCircle = {};

CCircleCollider::CCircleCollider()
{
	myCircleData = new Intersection::SCircle();
}

CCircleCollider::CCircleCollider(Intersection::SCircle* someCircleData)
{
	myCircleData = someCircleData;
}

CCircleCollider::CCircleCollider(CU::Vector3f aPosition, float aRadius)
{
	myCircleData = new Intersection::SCircle();
	myCircleData->myCenterPosition = aPosition;
	myCircleData->myRadius = aRadius;
}

CCircleCollider::CCircleCollider(const CCircleCollider& aCopy)
{
	self = aCopy;
}

CCircleCollider::CCircleCollider(CCircleCollider&& aTemporary)
{
	self = std::move(aTemporary);
}

CCircleCollider::~CCircleCollider()
{
	SAFE_DELETE(myCircleData);
}

CCircleCollider& CCircleCollider::operator=(const CCircleCollider& aCopy)
{
	myCircleData = new Intersection::SCircle(*aCopy.myCircleData);
	return self;
}

CCircleCollider& CCircleCollider::operator=(CCircleCollider&& aTemporary)
{
	myCircleData = aTemporary.myCircleData;
	aTemporary.myCircleData = nullptr;

	return self;
}

bool CCircleCollider::TestCollision(ICollider* aCollider)
{
	return aCollider->TestCollision(this);
}

bool CCircleCollider::TestCollision(CPointCollider* aPointCollider)
{
	return Intersection::PointInsideCircle(GetData(), aPointCollider->GetData());
}

bool CCircleCollider::TestCollision(CCircleCollider* aCircleCollider)
{
	return Intersection::CircleVsCircle(GetData(), aCircleCollider->GetData());
}

bool CCircleCollider::TestCollision(CSquareCollider* aSquareCollider)
{
	assert(!"point collider not implemented");
	return false;
}

bool CCircleCollider::TestCollision(CGroupCollider* aGroupCollider)
{
	return aGroupCollider->TestCollision(this);
}

void CCircleCollider::SetPosition(const CU::Vector3f& aPosition)
{
	if (myCircleData != nullptr)
	{
		myCircleData->myCenterPosition.Set(aPosition.x, aPosition.z);
	}
}

void CCircleCollider::SetPosition(const CU::Vector2f aPosition)
{
	if (myCircleData != nullptr)
	{
		myCircleData->myCenterPosition = aPosition;
	}
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
