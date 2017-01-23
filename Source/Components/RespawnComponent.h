#pragma once
#include "Component.h"

class RespawnComponent : public CComponent
{
public:
	RespawnComponent();
	~RespawnComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

	void Update(const float aDeltaTime);

private:
	bool myHasDied;

	float myElapsedTime;
	float myRespawnTime;
};

