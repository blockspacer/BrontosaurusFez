#include "stdafx.h"
#include "EnemyObjectiveComponent.h"
#include "../PostMaster/UpdateQuestEvent.h"


CEnemyObjectiveComponent::CEnemyObjectiveComponent(const std::string& aQuestName) : myQuestName(aQuestName)
{
	myType = eComponentType::eEnemyObjective;
}

CEnemyObjectiveComponent::~CEnemyObjectiveComponent()
{
}

void CEnemyObjectiveComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (aMessageType == eComponentMessageType::eDied)
	{
		CUpdateQuestEvent questEvent(myQuestName);

		PostMaster::GetInstance().SendLetter(eMessageType::QuestRelated, questEvent);
	}
}

void CEnemyObjectiveComponent::Destroy()
{

}
