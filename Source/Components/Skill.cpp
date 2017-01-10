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
#include "SkillComponent.h"
#include "SkillData.h"
Skill::Skill(SkillData* aSkillDataPointer)
{
	mySkillData = aSkillDataPointer;
	myIsActive = false;
	myIsSelected = false;
    myUpdateFunction = std::bind(&Skill::BasicAttackUpdate, this, std::placeholders::_1);
	myUser = nullptr;
	myColliderObject = SkillSystemComponentManager::GetInstance().GetGameObjectManager()->CreateGameObject();
	Intersection::CollisionData circleCollisionData = Intersection::CollisionData();
	circleCollisionData.myCircleData = new Intersection::SCircle;
	circleCollisionData.myCircleData->myCenterPosition = myColliderObject->GetWorldPosition();
	circleCollisionData.myCircleData->myRadius = 200.0f;
	CCollisionComponent* collisionComponent = SkillSystemComponentManager::GetInstance().GetCollisionComponentManager()->CreateCollisionComponent(CCollisionComponentManager::eColliderType::eCircle, circleCollisionData);
	collisionComponent->AddCollidsWith(eColliderType::eColliderType_Enemy | eColliderType::eColliderType_Player);
	collisionComponent->SetColliderType(eColliderType::eColliderType_Skill);
	myColliderObject->AddComponent(collisionComponent);
	collisionComponent->DeactivateCollider();
	collisionComponent->GetCollider()->SetGameObject(myColliderObject);
	//ToDo Deactivate collider; Move this piece of shit to a better place.

	myElapsedCoolDownTime = mySkillData->coolDown;
	
	myAnimationTimeElapsed = 0.f;
}


Skill::~Skill()
{
}

void Skill::Activate()
{
	if (myElapsedCoolDownTime >= mySkillData->coolDown)
	{
		myIsActive = true;
		OnActivation();
	
	}
}

void Skill::Deactivate()
{
	myIsActive = false;
	myIsSelected = false;
	myElapsedCoolDownTime = 0.0f;
	OnDeActivation();
}

void Skill::Update(float aDeltaTime)
{
	myElapsedCoolDownTime += aDeltaTime;


	if(myIsActive == true)
	{
		if (myAnimationTimeElapsed > mySkillData->animationDuration)
		{
			ActivateCollider(); // Remove this later on and replace it with animation wait time.
			myAnimationTimeElapsed = 0.f;
		}

		myUpdateFunction(aDeltaTime);
	
	}
}

void Skill::Init(CGameObject * aUser)
{
	myUser = aUser;
	myColliderObject->AddComponent(new SkillComponent(myUser, mySkillData));
}

void Skill::BasicAttackUpdate(float aDeltaTime)
{
	if (myIsActive == false)
	{
		return;
	}

	if(myTargetObject != nullptr)
	{
		if((myUser->GetWorldPosition() - myTargetObject->GetWorldPosition()).Length2() < mySkillData->activationRadius *  mySkillData->activationRadius)
		{
			eComponentMessageType type = eComponentMessageType::eStopMovement;
			myUser->NotifyComponents(type, SComponentMessageData());
			SComponentMessageData statedAttackingMessage;
			statedAttackingMessage.myString = "attack";
			myUser->NotifyComponents(eComponentMessageType::eBasicAttack, statedAttackingMessage);
			myElapsedCoolDownTime = 0.0f;
			myColliderObject->SetWorldPosition(myTargetObject->GetWorldPosition());
			myAnimationTimeElapsed += aDeltaTime;
		}
	}
	else
	{
		SComponentMessageData statedAttackingMessage;
		statedAttackingMessage.myString = "attack";
		myUser->NotifyComponents(eComponentMessageType::eBasicAttack, statedAttackingMessage);
		myElapsedCoolDownTime = 0.0f;
		CU::Vector3f direction = myTargetPosition - myUser->GetWorldPosition();
		myAnimationTimeElapsed += aDeltaTime;
		if(direction.Length2() > mySkillData->range * mySkillData->range)
		{
			direction.Normalize();
			direction *= mySkillData->range;
			myColliderObject->SetWorldPosition(myUser->GetWorldPosition() + direction);
		}
		else
		{
			myColliderObject->SetWorldPosition(myTargetPosition);
		}
		//ActivateCollider(); // Remove this later on and replace it with animation wait time.
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
void Skill::ActivateCollider()
{
	DL_PRINT("Animation done");
	Deactivate();
	eComponentMessageType type = eComponentMessageType::eSetIsColliderActive;
	SComponentMessageData data;
	data.myBool = false;
	myColliderObject->NotifyComponents(type, data);
	data.myBool = true;
	myColliderObject->NotifyComponents(type, data);
}
void Skill::OnActivation()
{
	myAnimationTimeElapsed = 0.f;
	DL_PRINT("Animation started");
}

void Skill::OnDeActivation()
{
	SComponentMessageData statedAttackingMessage;
	statedAttackingMessage.myString = "idle";
	myUser->NotifyComponents(eComponentMessageType::eBasicAttack, statedAttackingMessage);
	myTargetObject = nullptr;
}

void Skill::Select()
{
	if(myElapsedCoolDownTime >= mySkillData->coolDown)
	{
		myIsSelected = true;
	}
}

void Skill::Deselect()
{
	myIsSelected = false;
}
