#pragma once
#include "Component.h"

class CPlayerHealthMessenger : public CComponent
{
public:
	CPlayerHealthMessenger();
	~CPlayerHealthMessenger();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
};
