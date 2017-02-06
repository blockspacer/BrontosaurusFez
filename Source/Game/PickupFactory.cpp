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
#include "../CommonUtilities/JsonValue.h"
#include "PollingStation.h"
#include "matrix33.h"
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
	CU::Vector3f newPosition = GenerateNewPosition(aPosition);
	healthGlobe->SetWorldPosition(newPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::HEALTH, myHealthDropHealValue, 0, 100);

	healthGlobe->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = newPosition;
	CollisionData.myCircleData->myRadius = myPickupRadius; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);
	collider->DeactivateCollider();

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
	CU::Vector3f newPosition = GenerateNewPosition(aPosition);
	manaGlobe->SetWorldPosition(newPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::GOLD, aAmountToDrop, 0, 100);

	manaGlobe->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = newPosition;
	CollisionData.myCircleData->myRadius = myPickupRadius; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);
	collider->DeactivateCollider();

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
	CU::Vector3f newPosition = GenerateNewPosition(aPosition);
	healthGlobe->SetWorldPosition(newPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::MANA, myManaDropRestoreValue, 0, 100);

	healthGlobe->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = newPosition;
	CollisionData.myCircleData->myRadius = myPickupRadius; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);
	collider->DeactivateCollider();

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
	CU::Vector3f newPosition = GenerateNewPosition(aPosition);
	hat->SetWorldPosition(newPosition);

	CPickupComponent* pickup = CPickupManager::GetInstance().CreatePickupComponent(ePickupType::HAT, aHatName, 0, 100);

	hat->AddComponent(pickup);

	Intersection::CollisionData CollisionData;

	CollisionData.myCircleData = new Intersection::SCircle;
	CollisionData.myCircleData->myCenterPosition = newPosition;
	CollisionData.myCircleData->myRadius = myPickupRadius; //should not be hard coded, maybe not a problem

	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, CollisionData);

	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->SetColliderType(eColliderType::eColliderType_PickUp);
	collider->DeactivateCollider();

	hat->AddComponent(collider);

	CModelComponent* model = CModelComponentManager::GetInstance().CreateComponent("Models/Player/hat_basic.fbx"); // ändra till hat

	hat->AddComponent(model);

	myObjects.Add(hat);
	myComponents.Add(pickup);
	myComponents.Add(collider);
}



void CPickupFactory::Init(const std::string& aKey)
{
	CU::CJsonValue levelsFile;
	const std::string& errorString = levelsFile.Parse("Json/Drops.json");

	CU::CJsonValue levelsArray = levelsFile.at(aKey);
	if (levelsArray.IsNull() == true)
	{
		return;
	}

	myHealthDropHealValue = levelsArray.at("healthDropRestorePercentage").GetFloat();
	myManaDropRestoreValue = levelsArray.at("manaDropRestorePercentage").GetFloat();
	myPickupRadius = levelsArray.at("pickupRadius").GetFloat();
	
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

CU::Vector3f CPickupFactory::GenerateNewPosition(CU::Vector3f& aPosition)
{
	CU::Vector3f direction = aPosition - PollingStation::playerObject->GetWorldPosition();
	CU::Vector3f newPosition;
	direction.Normalize();
	float angle = rand() % 180;
	angle - 90.0f;
	angle *= PI / 180.0f;
	direction = direction * CU::Matrix33f::CreateRotateAroundZ(angle);
	newPosition = aPosition + direction * 50.0f;
	DL_PRINT("Drop angle %f", angle * 180.0f / PI);
	return newPosition;
}
