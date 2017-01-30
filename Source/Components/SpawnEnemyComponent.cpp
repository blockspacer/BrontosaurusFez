#include "stdafx.h"
#include "SpawnEnemyComponent.h"
#include "../Game/EnemyFactory.h"


CSpawnEnemyOnDeathComponent::CSpawnEnemyOnDeathComponent(const unsigned char aNumberOfEnemiesToSpawn)
{
	myNumberOfEnemiesToSpawn = aNumberOfEnemiesToSpawn;
}


CSpawnEnemyOnDeathComponent::~CSpawnEnemyOnDeathComponent()
{
}

void CSpawnEnemyOnDeathComponent::Destroy()
{
}

void CSpawnEnemyOnDeathComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if (aMessageType == eComponentMessageType::eDied)
	{
		for (unsigned short i = 0; i < myNumberOfEnemiesToSpawn; i++)
		{
			CEnemyFactory::GetInstance().CreateEnemy(GetParent()->GetWorldPosition());
		}
	}
}
