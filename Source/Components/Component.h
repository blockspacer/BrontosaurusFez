#pragma once

typedef unsigned int ComponentId;
#define NULL_COMPONENT 0

class CGameObject;
struct SComponentMessageData;
enum class eComponentMessageType;

enum class eComponentType
{
	eModel,
	eDebugSphere,
	eCamera,
	eInventory,
	eMainStat,
	eStat,
	eNone,
};

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
	eComponentType myType;

private:
	CGameObject* myParent;
	ComponentId myId;
};

inline ComponentId CComponent::GetId() const
{
	return myId;
}

__forceinline CGameObject* CComponent::GetParent()
{
	return myParent;
}
