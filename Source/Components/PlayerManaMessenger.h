#pragma once
#include "Component.h"

class CPlayerManaMessenger : public CComponent
{
public:
	CPlayerManaMessenger();
	~CPlayerManaMessenger();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
};
