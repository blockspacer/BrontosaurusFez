#include "stdafx.h"
#include "Skill.h"
#include "GameObject.h"
#include "CollisionComponentManager.h"
#include "CollisionComponent.h"
#include "SkillSystemComponentManager.h"
#include "GameObjectManager.h"
#include "../Collision/Intersection.h"
#include "../Collision/ICollider.h"
#include "CollisionComponent.h"
Skill::Skill()
{
	myIsActive = false;
	myIsSelected = false;
    myUpdateFunction = std::bind(&Skill::BasicAttackUpdate, this, std::placeholders::_1);
	myUser = nullptr;
	myColliderObject = SkillSystemComponentManager::GetInstance().GetGameObjectManager()->CreateGameObject();
	Intersection::CollisionData circleCollisionData = Intersection::CollisionData();
	circleCollisionData.myCircleData = new Intersection::SCircle;
	circleCollisionData.myCircleData->myCenterPosition = myColliderObject->GetWorldPosition();
	circleCollisionData.myCircleData->myRadius = 100000.0f;
	CCollisionComponent* collisionComponent = SkillSystemComponentManager::GetInstance().GetCollisionComponentManager()->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, circleCollisionData);
	collisionComponent->AddCollidsWith(eColliderType::eColliderType_Actor);
	myColliderObject->AddComponent(collisionComponent);
	collisionComponent->DeactivateCollider();
	//ToDo Deactivate collider; Move this piece of shit to a better place.
}


Skill::~Skill()
{
}

void Skill::Activate()
{
	myIsActive = true;
	OnActivation();
}

void Skill::Deactivate()
{
	myIsActive = false;
	myIsSelected = false;
	OnDeActivation();
}

void Skill::Update(float aDeltaTime)
{
	myUpdateFunction(aDeltaTime);
}

void Skill::Init(CGameObject * aUser)
{
	myUser = aUser;
}

void Skill::BasicAttackUpdate(float aDeltaTime)
{
	if(myTargetObject != nullptr)
	{
		if(CU::Vector3f(myUser->GetWorldPosition() - myTargetObject->GetWorldPosition()).Length2() < 50.0f * 50.0f)
		{
			eComponentMessageType type = eComponentMessageType::eStopMovement;
			myUser->NotifyComponents(type, SComponentMessageData());
			//TODO start Attack Animation
			SComponentMessageData statedAttackingMessage;
			statedAttackingMessage.myString = "attack";
			myUser->NotifyComponents(eComponentMessageType::eBasicAttack, statedAttackingMessage);
			//TODO: Activate Collider;
			type = eComponentMessageType::eSetIsColliderActive;
			SComponentMessageData data;
			data.myBool = true;
			myColliderObject->NotifyComponents(type, data);

		}
	}
}

void Skill::SetTargetPosition(CU::Vector3f aTargetPosition)
{	
	myTargetPosition = aTargetPosition;
}
void Skill::SetTargetObject(CGameObject* aTargetObject)
{
	myTargetObject = aTargetObject;
}
void Skill::OnActivation()
{

}

void Skill::OnDeActivation()
{
}
