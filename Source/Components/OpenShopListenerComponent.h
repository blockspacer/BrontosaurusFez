#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"

class COpenShopListenerComponent : public CComponent, public Subscriber
{
public:
	COpenShopListenerComponent();
	~COpenShopListenerComponent();
	void Reactivate();
private:
	bool myIsActive;

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	virtual void Destroy() override;

	// Inherited via Subscriber
	eMessageReturn Recieve(const Message & aMessage) override;
};

