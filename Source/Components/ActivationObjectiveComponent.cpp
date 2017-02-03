#include "stdafx.h"
#include "ActivationObjectiveComponent.h"
#include "../PostMaster/UpdateQuestEvent.h"


CActivationObjectiveComponent::CActivationObjectiveComponent(const std::string anObjectiveName) :myObjectiveName(anObjectiveName)
{
	myType = eComponentType::eActivationObjective;
}

CActivationObjectiveComponent::~CActivationObjectiveComponent()
{
}

void CActivationObjectiveComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (aMessageType == eComponentMessageType::eActivate2)
	{
		CUpdateQuestEvent questEvent(myObjectiveName);

		PostMaster::GetInstance().SendLetter(eMessageType::QuestRelated, questEvent);
	}
}

void CActivationObjectiveComponent::Destroy()
{

}
