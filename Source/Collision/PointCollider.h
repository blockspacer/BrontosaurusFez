#pragma once
#include "ICollider.h"

namespace Intersection
{
	struct SPoint;
}

class CPointCollider : public ICollider
{
public:
	CPointCollider();
	CPointCollider(const CU::Vector2f aPosition);
	CPointCollider(const CPointCollider& aCopy);
	~CPointCollider();

	bool TestCollision(ICollider* aCollider) override;
	bool TestCollision(CPointCollider* aPointCollider) override;
	bool TestCollision(CCircleCollider* aCircleCollider) override;
	bool TestCollision(CSquareCollider* aBoxCollider) override;
	bool TestCollision(CGroupCollider* aGroupCollider) override;

	const Intersection::SPoint& GetData() const;

private:
	Intersection::SPoint* myPointData;

	static Intersection::SPoint ourNullPoint;
};

