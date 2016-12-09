#pragma once
#include "Component.h"
//#include "../CommonUtilities/GrowingArray.h"
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/DynamicString.h"

class CGameObject :	public CComponent
{
public:
	friend class CObjectManager;
	CU::Matrix44f &GetLocalTransform();
	CU::Matrix44f GetToWorldTransform();
	CU::Vector3f GetWorlPosition();

	void AddComponent(CComponent* aComponent);

	//notifies all 
	void NotifyComponents(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData);

	void MarkForDestruction();

	void SetName(const CU::DynamicString& aName);
	CU::GrowingArray<CComponent*>& GetComponents();
	const CU::DynamicString& GetName()const;

private:
	void Destroy() override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData &aMessageData) override;
	CGameObject(CObjectManager& aManager);
	~CGameObject();

	ComponentId myTransformId;
	CU::GrowingArray<CComponent*> myComponents;
	CObjectManager& myManager;
	CU::DynamicString myName;
};


inline void CGameObject::SetName(const CU::DynamicString& aName)
{
	myName = aName;
}

inline CU::GrowingArray<CComponent*>& CGameObject::GetComponents()
{
	return myComponents;
}

inline const CU::DynamicString& CGameObject::GetName() const
{
	return myName;
}
