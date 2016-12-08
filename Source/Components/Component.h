#pragma once
#include "ComponentMessage.h"


typedef unsigned int ComponentId;
#define NULL_COMPONENT 0

class CGameObject;

class CComponent
{
public:
	friend class CComponentManager;
	friend class CGameObject;
	CComponent();
	virtual ~CComponent();
	
	//override this if you will need to get notified from other components
	virtual void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData);
	virtual void Destroy() = 0;

	__forceinline CGameObject* GetParent();

	ComponentId GetId()const;

protected:
	eComponentType myType = eComponentType::eNone;
private:

	CGameObject* myParent;
	ComponentId myId = NULL_COMPONENT;
};

inline ComponentId CComponent::GetId() const
{
	return myId;
}

__forceinline CGameObject* CComponent::GetParent()
{
	return myParent;
}
