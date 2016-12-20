#pragma once

#include <functional>

class CCircleCollider;
class CPointCollider;
class CSquareCollider;
class CGroupCollider;

class ICollider
{
public:
	using Callback = std::function<void(ICollider*)>;

	ICollider() : myOnEnterCallback(nullptr), myOnUpdateCallback(nullptr), myOnExitCallback(nullptr), myHasCollidedWith(nullptr) {}
	virtual ~ICollider() {}

	inline void InitCallbackFunctions(Callback aEnterCallback, Callback aUpdateCallback, Callback aExitCallback);

	virtual void RenderDebugLines() {}

	virtual bool TestCollision(ICollider* aCollider) = 0;
	virtual bool TestCollision(CPointCollider* aPointCollider) = 0;
	virtual bool TestCollision(CCircleCollider* aCircleCollider) = 0;
	virtual bool TestCollision(CSquareCollider* aBoxCollider) = 0;
	virtual bool TestCollision(CGroupCollider* aGroupCollider) = 0;

	virtual void SetPosition(const CU::Vector3f& aPosition) = 0;

	inline void OnCollisionEnter(ICollider* aOther);
	inline void OnCollisionUpdate(ICollider* aOther);
	inline void OnCollisionExit(ICollider* aOther);

	inline static bool HasCollided(ICollider* aFirst, ICollider* aSecond);

private:
	Callback myOnEnterCallback;
	Callback myOnUpdateCallback;
	Callback myOnExitCallback;

	ICollider* myHasCollidedWith;
};

inline bool ICollider::HasCollided(ICollider* aFirst, ICollider* aSecond)
{
	return aFirst->myHasCollidedWith == aSecond || aSecond->myHasCollidedWith == aFirst;
}

inline void ICollider::InitCallbackFunctions(Callback aEnterCallback, Callback aUpdateCallback, Callback aExitCallback)
{
	myOnEnterCallback = aEnterCallback;
	myOnUpdateCallback = aUpdateCallback;
	myOnExitCallback = aExitCallback;
}

inline void ICollider::OnCollisionEnter(ICollider* aOther)
{
	myHasCollidedWith = aOther;

	if (myOnEnterCallback != nullptr)
	{
		myOnEnterCallback(aOther);
	}
}

inline void ICollider::OnCollisionUpdate(ICollider* aOther)
{
	if (myOnUpdateCallback != nullptr)
	{
		myOnUpdateCallback(aOther);
	}
}

inline void ICollider::OnCollisionExit(ICollider* aOther)
{
	myHasCollidedWith = nullptr;

	if (myOnExitCallback != nullptr)
	{
		myOnExitCallback(aOther);
	}
}
