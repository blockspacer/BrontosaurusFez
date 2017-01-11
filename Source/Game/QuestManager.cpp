#include "stdafx.h"
#include "QuestManager.h"
#include "PostMaster/PostMaster.h"
#include "CommonUtilities.h"
#include "PostMaster/EMessageReturn.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"
#include "PostMaster/QuestDataUpdated.h"
QM::CQuestManager* QM::CQuestManager::ourInstance = nullptr;

void QM::CQuestManager::CreateInstance()
{
	if (ourInstance != nullptr)
	{
		DL_ASSERT("quest manager already created");
	}

	ourInstance = new CQuestManager();
}

QM::CQuestManager& QM::CQuestManager::GetInstance()
{
	if (ourInstance == nullptr)
	{
		DL_ASSERT("quest manager has not been created yet");
	}

	return *ourInstance;
}

void QM::CQuestManager::DestroyInstance()
{
	if (ourInstance != nullptr)
	{
		delete ourInstance;
		ourInstance = nullptr;
	}
}

void QM::CQuestManager::UpdateObjective(EventHandle anObjectiveHandle, int anAmmount)
{
	CU::GrowingArray<EventHandle> &currentObjectives = myCurrentObjectives.myObjectives;

	for (int i = 0; i < currentObjectives.Size(); ++i)
	{
		if (currentObjectives[i] == anObjectiveHandle)
		{
			SObjective& currentObjective = myObjectives[anObjectiveHandle];

			currentObjective.myAmmount = MIN(currentObjective.myGoal, currentObjective.myAmmount + anAmmount);
		}
	}

	SendUpdateMessage();
}

bool QM::CQuestManager::CheckIfQuestComplete() const
{
	const CU::GrowingArray<EventHandle> & objectives = myCurrentObjectives.myObjectives;
	for (int i = 0; i < objectives.Size(); ++i)
	{
		const SObjective& objective = myObjectives[objectives[i]];
		if (objective.myAmmount < objective.myGoal)
		{
			return false;
		}
	}
	return true;
}

void QM::CQuestManager::CompleteEvent()
{
	if (myCurrentObjectives.myObjectives.Size() > 0 && CheckIfQuestComplete() == false)
	{
		return;
	}

	myCurrentObjectives = SQuest();

	const SEvent nextEvent = myEvents.Pop();

	switch (nextEvent.myType)
	{
	case eEventType::OBJECTIVE:
		myCurrentObjectives.myObjectives.Add(nextEvent.myHandle);
		break;
	case eEventType::QUEST:
		myCurrentObjectives = myQuests[nextEvent.myHandle];
		break;
	case eEventType::QUEST_LINE:
	default: 
		DL_ASSERT("not implemented yet");
		break;
	}
	SendUpdateMessage();
}

void QM::CQuestManager::AddEvent(const QM::eEventType anEventType, const QM::EventHandle anEventHandle)
{
	const SEvent tempEvent = { anEventType, anEventHandle };
	AddEvent(tempEvent);
}

void QM::CQuestManager::AddEvent(const SEvent& anEvent)
{
	myEvents.Push(anEvent);
}

QM::SQuest QM::CQuestManager::GetCurrentQuest() const
{
	return myCurrentObjectives;
}

eMessageReturn QM::CQuestManager::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

QM::CQuestManager::CQuestManager()
{
	myObjectives.Init(4);
	myQuests.Init(4);

	POSTMASTER.Subscribe(this, eMessageType::QuestRelated);
}


QM::CQuestManager::~CQuestManager()
{
	POSTMASTER.UnSubscribe(this, eMessageType::QuestRelated);
}

void QM::CQuestManager::SendUpdateMessage()
{
	CQuestDataUpdated thisEvent;

	PostMaster::GetInstance().SendLetter(eMessageType::QuestRelated, thisEvent);
}

QM::EventHandle QM::CQuestManager::AddObjective(SObjective anObjective)
{
	const EventHandle handle = myObjectives.Size();
	myObjectives.Add(anObjective);
	return handle;
}

QM::EventHandle QM::CQuestManager::AddQuest(SQuest anObjective)
{
	const EventHandle handle = myQuests.Size();
	myQuests.Add(anObjective);
	return handle;
}

QM::SObjective QM::CQuestManager::GetObjective(const int aObjective)
{
	return myObjectives[aObjective];
}
