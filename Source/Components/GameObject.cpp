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
	if (aComponent == nullptr)
	{
		DL_MESSAGE_BOX("trying to add null component to game object with id %u", GetId());
		return;
	}

	aComponent->myParent = this;
	myComponents.Add(aComponent);
	SComponentMessageData data;
	data.myComponentTypeAdded = aComponent->myType;
	Receive(eComponentMessageType::eAddComponent, data);
}

void CGameObject::RemoveComponent(CComponent * aComponent)
{
	myComponents.RemoveCyclic(aComponent);
}

void CGameObject::NotifyComponents(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if (myParent != nullptr)
	{
		myParent->NotifyComponents(aMessageType, aMessageData);
	}
	else
	{
		Receive(aMessageType, aMessageData);
	}
}

void  CGameObject::NotifyOnlyComponents(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData)
{
	ComponentReceive(aMessageType, aMessageData);
}

void CGameObject::MarkForDestruction()
{
	myManager.AddObjectForDestruction(this);
}

bool CGameObject::IsGameObject()
{
	return true;
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
	for (unsigned short i = 0; i < myComponents.Size(); i++)
	{
		myComponents[i]->Receive(aMessageType, aMessageData);
	}
}

CU::GrowingArray<CComponent*>& CGameObject::GetComponents()
{
	return myComponents;
}

void CGameObject::ComponentReceive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	for (unsigned int i = 0; i < myComponents.Size(); i++)
	{
		if (myComponents[i]->IsGameObject() == false)
		{
			myComponents[i]->Receive(aMessageType, aMessageData);
		}
	}

	//CComponent* test = nullptr;
	//for (unsigned int i = 0; i < myComponents.Size(); ++i)
	//{
	//	if(dynamic_cast<CGameObject*>(myComponents[i]) != nullptr)
	//	{
	//		return;
	//	}
	//	test = myComponents[i];
	//	test->Receive(aMessageType, aMessageData);
	//}
}

CGameObject::CGameObject(CGameObjectManager &aManager) : myTransformId(0), myManager(aManager)
{
	myType = eComponentType::eGameObject;
	myComponents.Init(4);
}

CGameObject::~CGameObject()
{
}
