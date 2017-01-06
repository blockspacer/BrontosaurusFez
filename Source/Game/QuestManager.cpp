#include "stdafx.h"
#include "QuestManager.h"
#include "PostMaster/PostMaster.h"
#include "CommonUtilities.h"
#include "PostMaster/EMessageReturn.h"
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

void QM::CQuestManager::CompleteEvent(EventHandle anObjectiveHandle)
{
	if (myCurrentObjectives.myObjectives.Size() > 0 && CheckIfQuestComplete() == false)
	{
		return;
	}

	const SEvent nextEvent = myEvents.Pop();

	switch (nextEvent.myType)
	{
	case eEventType::OBJECTIVE:
		{
			SQuest newQuest;
			newQuest.myObjectives.Add(nextEvent.myHandle);
			myCurrentObjectives = newQuest;
		}
		break;
	case eEventType::QUEST:
		myCurrentObjectives = myQuests[nextEvent.myHandle];
		break;
	case eEventType::QUEST_LINE:
	default: 
		DL_ASSERT("not implemented yet");
		break;
	}
}

QM::EventHandle QM::CQuestManager::AddEvent(const SObjective& anObjective)
{
	SEvent newEvent;
	newEvent.myType = eEventType::OBJECTIVE;
	newEvent.myHandle = AddObjective(anObjective);

	myEvents.Push(newEvent);
	return newEvent.myHandle;
}

QM::EventHandle QM::CQuestManager::AddEvent(SQuest aQuest)
{
	SEvent newEvent;
	newEvent.myType = eEventType::QUEST;
	newEvent.myHandle = AddQuest(aQuest);

	myEvents.Push(newEvent);
	return newEvent.myHandle;
}

QM::SQuest QM::CQuestManager::GetCurrentObjectives() const
{
	return myCurrentObjectives;
}

eMessageReturn QM::CQuestManager::Recieve(const Message& aMessage)
{
	return eMessageReturn::eContinue;
}

QM::CQuestManager::CQuestManager()
{
	myObjectives.Init(4);
	myQuests.Init(4);

	POSTMASTER.Subscribe(this, eMessageType::QuestRelated);
}


QM::CQuestManager::~CQuestManager()
{
	POSTMASTER.UnSubscribeEveryWhere(this);
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
