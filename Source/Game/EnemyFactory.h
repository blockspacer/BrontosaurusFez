#pragma once
#include "../CommonUtilities/vector3.h"

class CGameObjectManager;
class CCollisionComponentManager;
class CHealthBarComponentManager;

class CEnemyFactory
{
public:
	static void Create(CGameObjectManager& aGameObjectManager, CCollisionComponentManager& aCollisionComponentManager, CHealthBarComponentManager& aHealthbarManager);
	static void Destroy();
	static CEnemyFactory& GetInstance();

	void CreateEnemy(CU::Vector3f aPosition);

private:
	CEnemyFactory(CGameObjectManager& aGameObjectManager, CCollisionComponentManager& aCollisionComponentManager, CHealthBarComponentManager& aHealthbarManager);
	~CEnemyFactory();

private:
	static CEnemyFactory* ourInstance;

	CGameObjectManager* myGameObjectManager;
	CCollisionComponentManager* myCollisionComponentManager;
	CHealthBarComponentManager* myHealthBarManager;
};

