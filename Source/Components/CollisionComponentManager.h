#pragma once

namespace CU
{
	class Time;
}

class CCollisionComponent;
class CCollisionManager;
class ICollider; //move this to CColliderFactory in Collision project

class CCollisionComponentManager
{
public:
	enum class eColliderType
	{
		eCircle,
		ePoint,
		eSquare
	};

	CCollisionComponentManager();
	~CCollisionComponentManager();

	void Update();

	CCollisionComponent* CreateCollisionComponent(const eColliderType aColliderType);
	void DestroyCollisionComponent(CCollisionComponent* aCollisionComponent);

private:
	ICollider* CreateCollider(const eColliderType aColliderType); //move this to CColliderFactory in Collision project

	CU::GrowingArray<CCollisionComponent*, unsigned int, false> myCollisionComponents;
	CCollisionManager* myCollisionManager;
};
