#pragma once
#include "Component.h"
class COpenShopListenerComponent : public CComponent
{
public:
	COpenShopListenerComponent();
	~COpenShopListenerComponent();
private:
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	// Inherited via CComponent
	virtual void Destroy() override;
};

