#pragma once
#include "ICollider.h"

class CGroupCollider : public ICollider
{
public:
	using size_type = unsigned short;

	CGroupCollider();
	~CGroupCollider();

	void RenderDebugLines() override;

	bool TestCollision(ICollider* aCollider) override;
	bool TestCollision(CCircleCollider* aCircleCollider) override;
	bool TestCollision(CSquareCollider* aBoxCollider) override;
	bool TestCollision(CGroupCollider* aGroupCollider) override;

private:
	CU::GrowingArray<ICollider*, size_type, false> myColliders;
};
