#include "stdafx.h"
#include "LoadObjectives.h"
#include "KevinLoader/KevinLoader.h"
#include "EnemyObjectiveComponent.h"
#include "ComponentManager.h"
#include "Component.h"

int LoadEnemyObjective(KLoader::SLoadedComponentData ComponentData)
{
	const std::string questName = ComponentData.myData.at("QuestName").GetString();
	CEnemyObjectiveComponent* questComponent = new CEnemyObjectiveComponent(questName);

	return CComponentManager::GetInstance().RegisterComponent(questComponent);
}
