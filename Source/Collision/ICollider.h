#pragma once

#include <functional>

class CCircleCollider;
class CPointCollider;
class CSquareCollider;
class CGroupCollider;
class CGameObject;
namespace CU
{
	template <typename T>
	class Vector3;
	using Vector3f = Vector3<float>;

	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;
}

enum eColliderType : unsigned int
{
	eColliderType_None = 0,
	eColliderType_Actor = 1 << 0,
	eColliderType_Wall = 1 << 1,
	eColliderType_Mouse = 1 << 2,
	eColliderType_Skill = 1 << 4,
};

class ICollider
{
public:
	using Callback = std::function<void(ICollider*)>;
	using VoidCallback = std::function<void(void)>;

	ICollider() : myOnEnterCallback(nullptr), myOnUpdateCallback(nullptr), myOnExitCallback(nullptr), myHasCollidedWith(nullptr), myActivateCallback(nullptr), myDeactivateCallback(nullptr), myColliderType(eColliderType::eColliderType_None), myCollidesWith(0u) {}
	virtual ~ICollider() {}

	inline void InitCallbackFunctions(const Callback& aEnterCallback, const Callback& aUpdateCallback, const Callback& aExitCallback);
	inline void AddActivationCallbacks(VoidCallback aActivationCallback, VoidCallback aDeactivationCallback);
	inline void SetColliderType(const eColliderType aColliderType);
	inline void AddCollidsWith(const unsigned int aColliderTypes);
	inline void SetGameObject(CGameObject* aGameObject);
	inline CGameObject* GetGameObject();

	virtual void RenderDebugLines() {}

	virtual bool TestCollision(ICollider* aCollider) = 0;
	virtual bool TestCollision(CPointCollider* aPointCollider) = 0;
	virtual bool TestCollision(CCircleCollider* aCircleCollider) = 0;
	virtual bool TestCollision(CSquareCollider* aSquareCollider) = 0;
	virtual bool TestCollision(CGroupCollider* aGroupCollider) = 0;

	virtual void SetPosition(const CU::Vector3f& aPosition) = 0;
	virtual void SetPosition(const CU::Vector2f aPosition) = 0;

	inline void OnCollisionEnter(ICollider* aOther);
	inline void OnCollisionUpdate(ICollider* aOther);
	inline void OnCollisionExit(ICollider* aOther);

	inline void Activate();
	inline void Deactivate();

	inline static bool CanCollide(const ICollider* aFirst, const ICollider* aSecond);
	inline static bool HasCollided(const ICollider* aFirst, const ICollider* aSecond);

private:
	Callback myOnEnterCallback;
	Callback myOnUpdateCallback;
	Callback myOnExitCallback;

	VoidCallback myActivateCallback;
	VoidCallback myDeactivateCallback;

	ICollider* myHasCollidedWith;

	CGameObject* myColliderObject;

	eColliderType myColliderType;
	unsigned int myCollidesWith;
};

inline bool ICollider::CanCollide(const ICollider* aFirst, const ICollider* aSecond)
{
	return (aFirst->myColliderType & aSecond->myCollidesWith) > 0 || (aFirst->myCollidesWith & aSecond->myColliderType);
}

inline bool ICollider::HasCollided(const ICollider* aFirst, const ICollider* aSecond)
{
	return aFirst->myHasCollidedWith == aSecond || aSecond->myHasCollidedWith == aFirst;
}

inline void ICollider::InitCallbackFunctions(const Callback& aEnterCallback, const Callback& aUpdateCallback, const Callback& aExitCallback)
{
	myOnEnterCallback = aEnterCallback;
	myOnUpdateCallback = aUpdateCallback;
	myOnExitCallback = aExitCallback;
}

inline void ICollider::AddActivationCallbacks(VoidCallback aActivationCallback, VoidCallback aDeactivationCallback)
{
	myActivateCallback = aActivationCallback;
	myDeactivateCallback = aDeactivationCallback;
}

inline void ICollider::SetColliderType(const eColliderType aColliderType)
{
	myColliderType = aColliderType;
}

inline void ICollider::AddCollidsWith(const unsigned int aColliderTypes)
{
	myCollidesWith |= aColliderTypes;
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

inline void ICollider::Activate()
{
	if (myActivateCallback != nullptr)
	{
		myActivateCallback();
	}
}

inline void ICollider::Deactivate()
{
	if (myDeactivateCallback != nullptr)
	{
		myDeactivateCallback();
	}
}

inline void ICollider::SetGameObject(CGameObject* aGameObject)
{
	myColliderObject = aGameObject;
}

inline CGameObject* ICollider::GetGameObject()
{
	return myColliderObject;
}