#pragma once
#include "ICollider.h"

namespace Intersection
{
	struct SCircle;
}

class CCircleCollider :	public ICollider
{
public:
	CCircleCollider();
	CCircleCollider(const CCircleCollider& aCopy);
	~CCircleCollider();

	bool TestCollision(ICollider* aCollider) override;
	bool TestCollision(CPointCollider* aCollider) override;
	bool TestCollision(CCircleCollider* aCircleCollider) override;
	bool TestCollision(CSquareCollider* aBoxCollider) override;
	bool TestCollision(CGroupCollider* aGroupCollider) override;

	const Intersection::SCircle& GetData() const;

private:
	Intersection::SCircle* myCircleData;

	static Intersection::SCircle ourNullCircle;
};
