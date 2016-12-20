#pragma once
#include "Component.h"
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/DynamicString.h"

class CGameObject :	public CComponent
{
public:
	friend class CGameObjectManager;
	friend void CU::GrowingArray<CGameObject*>::DeleteAll();

	CU::Matrix44f &GetLocalTransform();
	CU::Matrix44f GetToWorldTransform();
	CU::Vector3f GetWorlPosition();

	void AddComponent(CComponent* aComponent);

	//notifies all 
	void NotifyComponents(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData);

	void MarkForDestruction();

	inline void SetName(const CU::DynamicString& aName);
	inline const CU::DynamicString& GetName() const;

private:
	void Destroy() override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData) override;
	CGameObject(CGameObjectManager& aManager);
	~CGameObject();

	ComponentId myTransformId;
	CU::GrowingArray<CComponent*> myComponents;
	CGameObjectManager& myManager;
	CU::DynamicString myName;
};


inline void CGameObject::SetName(const CU::DynamicString& aName)
{
	myName = aName;
}

inline const CU::DynamicString& CGameObject::GetName() const
{
	return myName;
}
