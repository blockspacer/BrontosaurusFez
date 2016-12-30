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
	CCircleCollider(CCircleCollider&& aTemporary);
	~CCircleCollider();

	CCircleCollider& operator=(const CCircleCollider& aCopy);
	CCircleCollider& operator=(CCircleCollider&& aTemporary);

	bool TestCollision(ICollider* aCollider) override;
	bool TestCollision(CPointCollider* aCollider) override;
	bool TestCollision(CCircleCollider* aCircleCollider) override;
	bool TestCollision(CSquareCollider* aBoxCollider) override;
	bool TestCollision(CGroupCollider* aGroupCollider) override;

	void SetPosition(const CU::Vector3f& aPosition) override;
	void SetPosition(const CU::Vector2f aPosition) override;
	
	const Intersection::SCircle& GetData() const;

private:
	Intersection::SCircle* myCircleData;

	const static Intersection::SCircle ourNullCircle;
};
