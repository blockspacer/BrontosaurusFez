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
	void Init(const std::string& aKey);

private:
	CEnemyFactory(CGameObjectManager& aGameObjectManager, CCollisionComponentManager& aCollisionComponentManager, CHealthBarComponentManager& aHealthbarManager);
	~CEnemyFactory();

private:
	static CEnemyFactory* ourInstance;

	CGameObjectManager* myGameObjectManager;
	CCollisionComponentManager* myCollisionComponentManager;
	CHealthBarComponentManager* myHealthBarManager;
	std::string myEnemiesAttack;
	unsigned short myEnemiesHealth;
	unsigned short myEnemiesMana;
	unsigned short myEnemiesAmountOfGoldToDrop;
	float myEnemiesAggroRange;
	float myEnemiesSeekBeheviourWeight;
	float myEnemiesMaxSpeed;
	float myEnemiesMaxAcceleration;
	float myEnemiesSlowDownRadius;
	float myEnemiesTargetRadius;
	float myEnemiesCollisionRadius;
	unsigned char myEnemiesHealthDropChance;
	unsigned char myEnemiesManaDropChance;

};

