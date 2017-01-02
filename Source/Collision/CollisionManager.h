#pragma once

class ICollider;

class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

	void Update();
	void Render();

	void AddCollider(ICollider* aCollider);
	void RemoveCollider(ICollider* aCollider);

private:
	inline void TestCollision(ICollider* aFirst, ICollider* aSecond);

	CU::GrowingArray<ICollider*, int, false> myColliders;
	bool myShouldRenderDebugLines;
};
