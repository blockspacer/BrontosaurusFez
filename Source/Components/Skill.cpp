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
	collisionComponent->AddCollidsWith(eColliderType::eColliderType_Enemy | eColliderType::eColliderType_Player);
	collisionComponent->SetColliderType(eColliderType::eColliderType_Skill);
	myColliderObject->AddComponent(collisionComponent);
	collisionComponent->DeactivateCollider();
	collisionComponent->GetCollider()->SetGameObject(myColliderObject);
	//ToDo Deactivate collider; Move this piece of shit to a better place.
	myCoolDown = 0.5f;
	myElapsedCoolDownTime = myCoolDown;
	myRange = 300.0f;
	myAttackRadius2 = 160.0f * 160.0f;

	myAnimationTime = 0.5f;
	myAnimationTimeElapsed = 0.f;
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
	myElapsedCoolDownTime += aDeltaTime;

	if (myAnimationTimeElapsed > myAnimationTime)
	{
		ActivateCollider(); // Remove this later on and replace it with animation wait time.
		myAnimationTimeElapsed = 0.f;
	}

	myUpdateFunction(aDeltaTime);
}

void Skill::Init(CGameObject * aUser)
{
	myUser = aUser;
	myColliderObject->AddComponent(new SkillComponent(myUser));
}

void Skill::BasicAttackUpdate(float aDeltaTime)
{
	if (myIsActive == false)
	{
		return;
	}

	if(myTargetObject != nullptr)
	{
		if((myUser->GetWorldPosition() - myTargetObject->GetWorldPosition()).Length2() < myAttackRadius2)
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
		if(direction.Length2() > myRange * myRange)
		{
			direction.Normalize();
			direction *= myRange;
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
}

void Skill::Select()
{
	if(myElapsedCoolDownTime >= myCoolDown)
	{
		myIsSelected = true;
	}
}

void Skill::Deselect()
{
	myIsSelected = false;
}
