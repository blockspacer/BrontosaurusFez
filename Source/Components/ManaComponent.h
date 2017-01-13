#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"
#include "ManaPoint.h"

class ManaComponent : public CComponent
{
public:
	ManaComponent(ManaPoint aMaxAmountOfMana);
	~ManaComponent();
	float GetPrecentLeft() const;

	void SetMana(const ManaPoint aValue);
	void SetMaxMana(const ManaPoint aValue);
	void Init();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	float myPercentageLeft;
	ManaPoint myMana;
	ManaPoint myMaxMana;

};

