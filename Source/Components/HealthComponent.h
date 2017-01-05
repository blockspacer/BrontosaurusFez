#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"
typedef int HealthPoint;

class CHealthComponent : public CComponent, public Subscriber
{
public:
	CHealthComponent();
	~CHealthComponent();
	float GetPrecentLeft() const;
	
	void SetHealth(const HealthPoint aValue);
	void SetMaxHealth(const HealthPoint aValue);
	void Init();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	float myPercentageLeft;
	HealthPoint myHealth;
	HealthPoint myMaxHealth;

	virtual eMessageReturn Recieve(const Message & aMessage) override;
};

