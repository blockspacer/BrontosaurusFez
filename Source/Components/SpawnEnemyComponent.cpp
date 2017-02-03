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
			CU::Vector3f temp(GetParent()->GetWorldPosition());

			temp.x += i*50;
			temp.z += i*50;

			CEnemyFactory::GetInstance().CreateEnemy(temp);
		}
	}
}
