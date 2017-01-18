#pragma once
#include "../CommonUtilities/vector3.h"


class CGameObjectManager;
class CCollisionComponentManager;
class CComponent;
class CGameObject;

class CPickupFactory
{
public:
	static void Create(CGameObjectManager* aCGameObjectManager, CCollisionComponentManager* aManager);
	static void Destroy();
	static CPickupFactory& GetInstance();


	void CreateHealthGlobe(CU::Vector3f aPosition);

private:
	void Init();

	CPickupFactory(CGameObjectManager* aCGameObjectManager, CCollisionComponentManager* aManager);
	~CPickupFactory();



private:

	CU::GrowingArray<CGameObject*> myObjects;
	CU::GrowingArray<CComponent*> myComponents;

	static CPickupFactory* ourInstance;
	CGameObjectManager* myGameObjectManager;
	CCollisionComponentManager* myCollisionComponentManager;
	std::string myModelPath;

};

