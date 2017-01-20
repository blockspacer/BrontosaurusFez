#include "stdafx.h"
#include "UpdateQuestEvent.h"
#include "Game/QuestManager.h"


CUpdateQuestEvent::CUpdateQuestEvent(std::string aQuestName, const int anAmmount) : myQuestName(aQuestName), myAmmount(anAmmount)
{
}

CUpdateQuestEvent::~CUpdateQuestEvent()
{
}

eMessageReturn CUpdateQuestEvent::DoEvent(QM::CQuestManager* aQuestManager) const
{
	const QM::EventHandle questHandle = aQuestManager->GetObjectiveHandle(myQuestName);
	aQuestManager->UpdateObjective(questHandle, myAmmount);
	return eMessageReturn::eContinue;
}
