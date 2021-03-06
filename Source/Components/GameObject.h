#pragma once
#include "Component.h"
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/DynamicString.h"

#ifndef _RETAIL_BUILD
#define NAMED_GAME_OBJECTS
#endif // !_RETAIL_BUILD

class CGameObject :	public CComponent
{
public:
	friend class CGameObjectManager;
	friend void CU::GrowingArray<CGameObject*>::DeleteAll();

	CU::Matrix44f &GetLocalTransform();
	CU::Matrix44f GetToWorldTransform();
	CU::Vector3f GetWorldPosition();

	//super derp ta bort n�sta projekt - Johan
	CU::GrowingArray<CComponent*>& GetComponents();

	void SetWorldPosition(CU::Vector3f);

	void AddComponent(CComponent* aComponent);
	void RemoveComponent(CComponent* aComponent);
	//notifies all 
	void NotifyComponents(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData);
	void NotifyOnlyComponents(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData);

	void MarkForDestruction();
	bool IsGameObject() override;

	inline void SetName(const CU::DynamicString& aName);
	inline const CU::DynamicString& GetName() const;

private:
	void Destroy() override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData) override;
	void ComponentReceive(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData);
	CGameObject(CGameObjectManager& aManager);
	CGameObject(const CGameObject& anObject) = delete;
	~CGameObject();

	CGameObject& operator=(const CGameObject& anObject) = delete;

	ComponentId myTransformId;
	CU::GrowingArray<CComponent*> myComponents;
	CGameObjectManager& myManager;
	CU::DynamicString myName;
};


inline void CGameObject::SetName(const CU::DynamicString& aName)
{
#ifdef NAMED_GAME_OBJECTS
	myName = aName;
#else
	aName;
#endif // NAMED_GAME_OBJECTS
}

inline const CU::DynamicString& CGameObject::GetName() const
{
#ifdef NAMED_GAME_OBJECTS
	return myName;
#else
	return "";
#endif // NAMED_GAME_OBJECTS
}
