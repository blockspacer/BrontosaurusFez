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
#include "SkillComponentManager.h"
Skill::Skill(SkillData* aSkillDataPointer)
{
	if (aSkillDataPointer->skillName == SkillData::SkillName::BasicAttack)
	{
		myUpdateFunction = std::bind(&Skill::BasicAttackUpdate, this, std::placeholders::_1);
	}
	else if (aSkillDataPointer->skillName == SkillData::SkillName::WhirlWind)
	{
		myUpdateFunction = std::bind(&Skill::WhirlWindUpdate, this, std::placeholders::_1);
	}
	else if (aSkillDataPointer->skillName == SkillData::SkillName::SweepAttack)
	{
		myUpdateFunction = std::bind(&Skill::SweepAttackUpdate, this, std::placeholders::_1);
	}
	else
	{
		DL_PRINT("Wow Skill couldn't find what skill to use as updatefunction. Check spelling and/or yell at Marcus.");
	}
	mySkillData = aSkillDataPointer;
	myIsActive = false;
	myIsSelected = false;
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
	myColliderObject->AddComponent(SkillComponentManager::GetInstance().CreateAndRegisterComponent(myUser, mySkillData));
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
			myColliderObject->GetLocalTransform().SetPosition(myTargetObject->GetWorldPosition());
			myColliderObject->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
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
			myColliderObject->GetLocalTransform().SetPosition(myTargetPosition);
		}
		myColliderObject->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
		//ActivateCollider(); // Remove this later on and replace it with animation wait time.
	}
}

void Skill::WhirlWindUpdate(float aDeltaTime)
{
	SComponentMessageData statedAttackingMessage;
	statedAttackingMessage.myString = "turnRight90";
	myUser->NotifyComponents(eComponentMessageType::eBasicAttack, statedAttackingMessage);
	myElapsedCoolDownTime = 0.0f;
	myAnimationTimeElapsed += aDeltaTime;
	myColliderObject->GetLocalTransform().SetPosition(myUser->GetWorldPosition());
	myColliderObject->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
}

void Skill::SweepAttackUpdate(float aDeltaTime)
{
	myUser->NotifyComponents(eComponentMessageType::eStopMovement, SComponentMessageData());
	SComponentMessageData startedAttackingMessage;
	startedAttackingMessage.myString = "turnLeft90";
	myUser->NotifyComponents(eComponentMessageType::eBasicAttack, startedAttackingMessage);
	myElapsedCoolDownTime = 0.0f;
	myAnimationTimeElapsed += aDeltaTime;
	CU::Vector3f direction = myTargetPosition - myUser->GetWorldPosition();
	if (direction.Length2() > mySkillData->range * mySkillData->range)
	{
		direction.Normalize();
		direction *= mySkillData->range;
		myColliderObject->SetWorldPosition(myUser->GetWorldPosition() + direction);
	}
	else
	{
		myColliderObject->GetLocalTransform().SetPosition(myTargetPosition);
	}
	myColliderObject->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
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
	//DL_PRINT("Animation done");
	Deactivate();
	eComponentMessageType type = eComponentMessageType::eSetIsColliderActive;
	SComponentMessageData data;
	
	data.myBool = true;
	myColliderObject->NotifyComponents(type, data);
}
void Skill::OnActivation()
{
	myAnimationTimeElapsed = 0.f;
	//DL_PRINT("Animation started");
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
		if(mySkillData->isChannel == true)
		{
			Activate();
		}
	}
}

void Skill::Deselect()
{
	myIsSelected = false;
}
