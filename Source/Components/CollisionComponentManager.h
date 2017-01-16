#pragma once

namespace CU
{
	class Time;
}

class CCollisionComponent;
class CCollisionManager;
class ICollider; //move this to CColliderFactory in Collision project
namespace Intersection
{
	union CollisionData;

}
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
	void Render();

	CCollisionComponent* CreateCollisionComponent(const eColliderType aColliderType, Intersection::CollisionData& aCollisionData);
	void DestroyCollisionComponent(CCollisionComponent* aCollisionComponent);

private:
	ICollider* CreateCollider(const eColliderType aColliderType, Intersection::CollisionData& aCollisionData); //move this to CColliderFactory in Collision project

	CU::GrowingArray<CCollisionComponent*, unsigned int, false> myCollisionComponents;
	CCollisionManager* myCollisionManager;
};
