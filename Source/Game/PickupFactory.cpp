#include "stdafx.h"
#include "PickupFactory.h"
#include "../Components/GameObject.h"
#include "../Components/GameObjectManager.h"
#include "../Components/CollisionComponent.h"
#include "../Components/CollisionComponentManager.h"
#include "../Components/Component.h"
#include "../Components/ModelComponent.h"
#include "../Components/ModelComponentManager.h"

#include "../Collision/Intersection.h"
#include "../Collision/ICollider.h"

#include "PickupManager.h"
#include "PickupComponent.h"

#include "../CommonUtilities/matrix44.h"

CPickupFactory* CPickupFactory::ourInstance = nullptr;

void CPickupFactory::Create(CGameObjectManager* aCGameObjectManager, CCollisionComponentManager* aManager)
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CPickupFactory(aCGameObjectManager, aManager);
	}
}

void CPickupFactory::Destroy()
{
	SAFE_DELETE(ourInstance);
}

CPickupFactory & CPickupFactory::GetInstance()
{
	return *ourInstance;
}

void CPickupFactory::CreateHealthGlobe(CU::Vector3f aPosition)
{
	CGameObject* healthGlobe = myGameObjectManager->CreateGameObject();
	healthGlobe->SetWorldPosition(aPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::HEALTH, 100, 0, 100);

	healthGlobe->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = aPosition;
	CollisionData.myCircleData->myRadius = 40; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);

	healthGlobe->AddComponent(collider);

	CModelComponent* model = CModelComponentManager::GetInstance().CreateComponent("Models/Pickups/healthSphere.fbx");

	healthGlobe->AddComponent(model);

	myObjects.Add(healthGlobe);
	myComponents.Add(pickup);
	myComponents.Add(collider);
}

void CPickupFactory::CreateGoldPickup(CU::Vector3f aPosition, const unsigned int aAmountToDrop)
{
	CGameObject* manaGlobe = myGameObjectManager->CreateGameObject();
	manaGlobe->SetWorldPosition(aPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::GOLD, aAmountToDrop, 0, 100);

	manaGlobe->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = aPosition;
	CollisionData.myCircleData->myRadius = 40; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);

	manaGlobe->AddComponent(collider);

	CModelComponent* model = CModelComponentManager::GetInstance().CreateComponent("Models/Placeholders/goldCoins.fbx");

	manaGlobe->AddComponent(model);

	myObjects.Add(manaGlobe);
	myComponents.Add(pickup);
	myComponents.Add(collider);
}

void CPickupFactory::CreateManaGlobe(CU::Vector3f aPosition)
{
	CGameObject* healthGlobe = myGameObjectManager->CreateGameObject();
	healthGlobe->SetWorldPosition(aPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::MANA, 100, 0, 100);

	healthGlobe->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = aPosition;
	CollisionData.myCircleData->myRadius = 40; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);

	healthGlobe->AddComponent(collider);

	CModelComponent* model = CModelComponentManager::GetInstance().CreateComponent("Models/Pickups/manaSphere.fbx");

	healthGlobe->AddComponent(model);

	myObjects.Add(healthGlobe);
	myComponents.Add(pickup);
	myComponents.Add(collider);
}

void CPickupFactory::CreateHatDrop(CU::Vector3f aPosition, const char* aHatName)
{
	CGameObject* hat = myGameObjectManager->CreateGameObject();
	hat->SetWorldPosition(aPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::HAT, aHatName, 0, 100);

	hat->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = aPosition;
	CollisionData.myCircleData->myRadius = 40; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);

	hat->AddComponent(collider);

	CModelComponent* model = CModelComponentManager::GetInstance().CreateComponent("Models/Player/hat_basic.fbx"); // ändra till hat

	hat->AddComponent(model);

	myObjects.Add(hat);
	myComponents.Add(pickup);
	myComponents.Add(collider);
}



void CPickupFactory::Init()
{
	//read from json doc and set modelpath and other things
}

CPickupFactory::CPickupFactory(CGameObjectManager* aCGameObjectManager, CCollisionComponentManager* aManager)
{
	myCollisionComponentManager = aManager;
	myGameObjectManager = aCGameObjectManager;
	myComponents.Init(10);
	myObjects.Init(10);
}


CPickupFactory::~CPickupFactory()
{
}
