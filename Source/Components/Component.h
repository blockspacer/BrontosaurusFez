#pragma once

typedef unsigned int ComponentId;
#define NULL_COMPONENT 0

class CGameObject;
struct SComponentMessageData;
enum class eComponentMessageType;

enum class eComponentType
{
	eGameObject,
	eModel,
	eDebugSphere,
	eCamera,
	eInventory,
	eHealthBar,
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
	
	void NotifyParent(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData);

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

inline CGameObject* CComponent::GetParent()
{
	return myParent;
}

inline ComponentId CComponent::GetId() const
{
	return myId;
}
