#include "stdafx.h"
#include "EnemyFactory.h"

#include "ComponentManager.h"

#include "../Collision/Intersection.h"
#include "Collision\ICollider.h"
#include "CollisionComponent.h"
#include "CollisionComponentManager.h"

#include "GameObject.h"
#include "GameObjectManager.h"

#include "HealthComponent.h"
#include "HealthBarComponent.h"
#include "HealthBarComponentManager.h"

#include "SkillSystemComponent.h"
#include "SkillSystemComponentManager.h"

#include "MovementComponent.h"
#include "MovementComponentManager.h"

#include "NavigationComponent.h"

#include "AIControllerComponent.h"
#include "AIControllerManager.h"

#include "SeekController.h"
#include "SeekControllerManager.h"

#include "AudioSourceComponent.h"
#include "AudioSourceComponentManager.h"

#include "DropComponent.h"
#include "DropComponentManager.h"

#include "ModelComponent.h"
#include "ModelComponentManager.h"

#include "ManaComponent.h"
#include "ManaComponentManager.h"

#include "../CommonUtilities/JsonValue.h"
#include "ComponentMessage.h"

CEnemyFactory* CEnemyFactory::ourInstance = nullptr;

void CEnemyFactory::Create(CGameObjectManager& aGameObjectManager, CCollisionComponentManager& aCollisionComponentManager, CHealthBarComponentManager& aHealthbarManager)
{
	assert(ourInstance == nullptr && "enemy factory not created");
	ourInstance = new CEnemyFactory(aGameObjectManager,aCollisionComponentManager, aHealthbarManager);
}

void CEnemyFactory::Destroy()
{
	SAFE_DELETE(ourInstance);
}

CEnemyFactory & CEnemyFactory::GetInstance()
{
	return *ourInstance;
}

void CEnemyFactory::CreateEnemy(CU::Vector3f aPosition)
{
	CGameObject* Enemy = myGameObjectManager->CreateGameObject();
	Enemy->SetWorldPosition(aPosition);

	CModelComponent* model = CModelComponentManager::GetInstance().CreateComponent("Models/StandardEnemy/standardEnemy.fbx"); //json
	Enemy->AddComponent(model);

	CHealthComponent* health = new CHealthComponent();
	health->SetMaxHealth(myEnemiesHealth); //json
	health->SetObjectType(eObjectType::eZombie);
	CComponentManager::GetInstance().RegisterComponent(health);
	Enemy->AddComponent(health);

	CHealthBarComponent* healthBar = myHealthBarManager->CreateHealthbar();
	Enemy->AddComponent(healthBar);

	ManaComponent* mana = ManaComponentManager::GetInstance().CreateAndRegisterComponent(myEnemiesMana); // json
	Enemy->AddComponent(mana);

	CSeekController* seek = CSeekControllerManager::GetInstance().CreateAndRegister();
	//from json
	//{
	seek->SetMaxSpeed(myEnemiesMaxSpeed);
	seek->SetAggroRange(myEnemiesAggroRange);
	seek->SetTargetRadius(myEnemiesTargetRadius);
	seek->SetWeight(myEnemiesSeekBeheviourWeight);
	seek->SetMaxAcceleration(myEnemiesMaxAcceleration);
	seek->SetSlowDownRadius(myEnemiesSlowDownRadius);
	//}
	Enemy->AddComponent(seek);

	CAIControllerComponent* AIControl = new CAIControllerComponent();
	AIControllerManager::GetInstance().AddController(AIControl);
	CComponentManager::GetInstance().RegisterComponent(AIControl);
	AIControl->AddControllerBehaviour(seek);

	Enemy->AddComponent(AIControl);

	DropComponent* drop = DropComponentManager::GetInstance().CreateAndRegisterComponent(myEnemiesAmountOfGoldToDrop, myEnemiesHealthDropChance,myEnemiesManaDropChance,""); // from json
	Enemy->AddComponent(drop);

	SkillSystemComponent* skillSystem = new SkillSystemComponent();
	//for skill in json
	skillSystem->AddSkill(myEnemiesAttack.c_str());

	SkillSystemComponentManager::GetInstance().RegisterComponent(skillSystem);
	CComponentManager::GetInstance().RegisterComponent(skillSystem);
	Enemy->AddComponent(skillSystem);

	Intersection::CollisionData collisionData;
	collisionData.myCircleData = new Intersection::SCircle;
	collisionData.myCircleData->myRadius = myEnemiesCollisionRadius; //set from json
	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, collisionData);
	collider->SetColliderType(eColliderType::eColliderType_Enemy);
	collider->AddCollidsWith(eColliderType::eColliderType_Mouse);
	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->AddCollidsWith(eColliderType::eColliderType_Skill);

	Enemy->AddComponent(collider);

	MovementComponent* movement = MovementComponentManager::GetInstance().CreateAndRegisterComponent();
	movement->SetMovementSpeed(myEnemiesMaxSpeed);

	Enemy->AddComponent(movement);

	NavigationComponent* navigation = new NavigationComponent();

	CComponentManager::GetInstance().RegisterComponent(navigation);

	Enemy->AddComponent(navigation);

	CAudioSourceComponent* audio = CAudioSourceComponentManager::GetInstance().CreateComponent();

	Enemy->AddComponent(audio);

	SComponentMessageData spawningData;
	spawningData.myString = "spawn";
	Enemy->NotifyOnlyComponents(eComponentMessageType::eBasicAttack, spawningData);

	Enemy->NotifyOnlyComponents(eComponentMessageType::eSpawning, SComponentMessageData());
}

void CEnemyFactory::Init(const std::string& aKey)
{
	CU::CJsonValue levelsFile;
	const std::string& errorString = levelsFile.Parse("Json/SpawnEnemies.json");

	if (!levelsFile.HasKey(aKey))
	{
		DL_MESSAGE_BOX("Level key '%s' not found in Json/SpawnEnemies.json", aKey.c_str());
		return;
	}

	CU::CJsonValue levelsArray = levelsFile.at(aKey);

	myEnemiesAttack = levelsArray.at("attack").GetString();
	myEnemiesHealth = levelsArray.at("health").GetUInt();
	myEnemiesMana = levelsArray.at("mana").GetUInt();
	myEnemiesAmountOfGoldToDrop = levelsArray.at("goldDrop").GetUInt();
	myEnemiesAggroRange = levelsArray.at("aggroRange").GetFloat();
	myEnemiesSeekBeheviourWeight = levelsArray.at("seekBeheviourWeight").GetFloat();
	myEnemiesMaxSpeed = levelsArray.at("maxSpeed").GetFloat();
	myEnemiesMaxAcceleration = levelsArray.at("maxAcceleration").GetFloat();
	myEnemiesSlowDownRadius = levelsArray.at("slowDownRadius").GetFloat();
	myEnemiesTargetRadius = levelsArray.at("targetRadius").GetFloat();
	myEnemiesCollisionRadius = levelsArray.at("collisionRadius").GetFloat();
	myEnemiesHealthDropChance = levelsArray.at("healthDropChance").GetUInt();
	myEnemiesManaDropChance = levelsArray.at("manaDropChance").GetUInt();
}

CEnemyFactory::CEnemyFactory(CGameObjectManager& aGameObjectManager, CCollisionComponentManager& aCollisionComponentManager, CHealthBarComponentManager& aHealthbarManager)
{
	myGameObjectManager = &aGameObjectManager;
	myCollisionComponentManager = &aCollisionComponentManager;
	myHealthBarManager = &aHealthbarManager;

}


CEnemyFactory::~CEnemyFactory()
{
}
