#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"
#include "HealthPoint.h"

enum class eObjectType
{
	eUrn,
	eBarrel,
	eWitch,
	eZombie,
	eBlob,
	ePlayer
};

class CHealthComponent : public CComponent, public Subscriber
{
public:
	CHealthComponent();
	~CHealthComponent();
	float GetPrecentLeft() const;
	
	void SetHealth(const HealthPoint aValue);
	void SetMaxHealth(const HealthPoint aValue);
	void SetObjectType(const eObjectType aType);
	void Init();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	eObjectType myObjectType;
	float myPercentageLeft;
	HealthPoint myHealth;
	HealthPoint myMaxHealth;

	virtual eMessageReturn Recieve(const Message & aMessage) override;
};

