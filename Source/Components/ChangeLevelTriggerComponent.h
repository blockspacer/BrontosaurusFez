#pragma once
#include "Component.h"

class ChangeLevelTriggerComponent : public CComponent
{
public:
	ChangeLevelTriggerComponent(unsigned char aLevelToGoTo);
	~ChangeLevelTriggerComponent();

private:
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy()override;

private:
	unsigned char myLevelToGoTo;
};

