#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectManager.h"


CU::Matrix44f& CGameObject::GetLocalTransform()
{
	return myManager.GetTransform(*this);
}

CU::Matrix44f CGameObject::GetToWorldTransform()
{
	if (GetParent() != nullptr)
	{
		return GetLocalTransform() * GetParent()->GetToWorldTransform();
	}
	else
	{
		return GetLocalTransform();
	}
}
CU::Vector3f CGameObject::GetWorldPosition()
{
	if (myParent != nullptr)
	{
		return GetLocalTransform().GetPosition() + GetParent()->GetWorldPosition();
	}
	else
	{
		return GetLocalTransform().GetPosition();
	}
}

void CGameObject::SetWorldPosition(CU::Vector3f aPosition)
{
	if (GetParent() != nullptr)
	{
		GetParent()->SetWorldPosition(GetLocalTransform().GetPosition() + aPosition);
	}
	else
	{
		GetLocalTransform().SetPosition(aPosition);
	}
}

void CGameObject::AddComponent(CComponent* aComponent)
{
	aComponent->myParent = this;
	myComponents.Add(aComponent);
	SComponentMessageData data;
	data.myComponentTypeAdded = aComponent->myType;
	Receive(eComponentMessageType::eAddComponent, data);
}

void CGameObject::NotifyComponents(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if (myParent != nullptr)
		myParent->NotifyComponents(aMessageType, aMessageData);
	else
		Receive(aMessageType, aMessageData);
}

void CGameObject::MarkForDestruction()
{
	myManager.AddObjectForDestruction(this);
}

void CGameObject::Destroy()
{
	for (unsigned int i = 0; i < myComponents.Size(); ++i)
	{
		myComponents[i]->Destroy();
		myComponents[i] = nullptr;
	}

	myManager.DestroyObject(this);
}

void CGameObject::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	CComponent* test = nullptr;
	for (unsigned int i = 0; i < myComponents.Size(); ++i)
	{
		test = myComponents[i];
		test->Receive(aMessageType, aMessageData);
	}
}

CGameObject::CGameObject(CGameObjectManager &aManager) : myTransformId(0), myManager(aManager)
{
	myType = eComponentType::eGameObject;
	myComponents.Init(4);
}

CGameObject::~CGameObject()
{
	
}
