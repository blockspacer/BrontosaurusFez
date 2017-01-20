#pragma once
#include "Component.h"

class CActivationComponent : public CComponent
{
public:
	CActivationComponent();
	~CActivationComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	
};

