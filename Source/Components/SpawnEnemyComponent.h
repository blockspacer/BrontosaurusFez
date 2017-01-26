#pragma once
#include "Component.h"
class CSpawnEnemyOnDeathComponent : public CComponent
{
public:
	CSpawnEnemyOnDeathComponent();
	~CSpawnEnemyOnDeathComponent();

	void Destroy() override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;

private:
	unsigned char myNumberOfEnemiesToSpawnPerSpawn;

};

