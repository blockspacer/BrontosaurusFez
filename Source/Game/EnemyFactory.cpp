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

CEnemyFactory* CEnemyFactory::ourInstance = nullptr;

void CEnemyFactory::Create(CGameObjectManager& aGameObjectManager, CCollisionComponentManager& aCollisionComponentManager, CHealthBarComponentManager& aHealthbarManager)
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CEnemyFactory(aGameObjectManager,aCollisionComponentManager, aHealthbarManager);
	}
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
	health->SetMaxHealth(100); //json
	CComponentManager::GetInstance().RegisterComponent(health);
	Enemy->AddComponent(health);

	CHealthBarComponent* healthBar = myHealthBarManager->CreateHealthbar();
	Enemy->AddComponent(healthBar);

	ManaComponent* mana = ManaComponentManager::GetInstance().CreateAndRegisterComponent(100); // json
	Enemy->AddComponent(mana);

	CSeekController* seek = CSeekControllerManager::GetInstance().CreateAndRegister();
	//from json
	//{
	seek->SetAggroRange(1300);
	seek->SetWeight(3);
	seek->SetMaxSpeed(30);
	seek->SetMaxAcceleration(50);
	seek->SetSlowDownRadius(100);
	seek->SetTargetRadius(150);
	//}
	Enemy->AddComponent(seek);

	CAIControllerComponent* AIControl = new CAIControllerComponent();
	AIControllerManager::GetInstance().AddController(AIControl);
	CComponentManager::GetInstance().RegisterComponent(AIControl);
	AIControl->AddControllerBehaviour(seek);

	Enemy->AddComponent(AIControl);

	DropComponent* drop = DropComponentManager::GetInstance().CreateAndRegisterComponent(100, 20); // from json
	Enemy->AddComponent(drop);

	SkillSystemComponent* skillSystem = new SkillSystemComponent();
	//for skill in json
	skillSystem->AddSkill("EnemyAttack");

	SkillSystemComponentManager::GetInstance().RegisterComponent(skillSystem);
	CComponentManager::GetInstance().RegisterComponent(skillSystem);
	Enemy->AddComponent(skillSystem);

	Intersection::CollisionData collisionData;
	collisionData.myCircleData = new Intersection::SCircle;
	collisionData.myCircleData->myRadius = 200; //set from json
	CCollisionComponent* collider = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, collisionData);
	collider->SetColliderType(eColliderType::eColliderType_Enemy);
	collider->AddCollidsWith(eColliderType::eColliderType_Mouse);
	collider->AddCollidsWith(eColliderType::eColliderType_Player);
	collider->AddCollidsWith(eColliderType::eColliderType_Skill);

	Enemy->AddComponent(collider);

	MovementComponent* movement = MovementComponentManager::GetInstance().CreateAndRegisterComponent();

	Enemy->AddComponent(movement);

	NavigationComponent* navigation = new NavigationComponent();

	CComponentManager::GetInstance().RegisterComponent(navigation);

	Enemy->AddComponent(navigation);

	CAudioSourceComponent* audio = CAudioSourceComponentManager::GetInstance().CreateComponent();

	Enemy->AddComponent(audio);


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
