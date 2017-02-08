#pragma once
#include "Component.h"

class ChangeLevelTriggerComponent : public CComponent
{
public:
	ChangeLevelTriggerComponent(const unsigned char aLevelToGoTo,const unsigned short aPortalID);
	~ChangeLevelTriggerComponent();
	bool CheckIfActive();

private:
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy()override;

private:
	unsigned char myLevelToGoTo;
	unsigned short myPortalID;
	bool myIsActive;
};

