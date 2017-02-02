#include "stdafx.h"
#include "QuestManager.h"
#include "PostMaster/PostMaster.h"
#include "CommonUtilities.h"
#include "PostMaster/EMessageReturn.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"
#include "PostMaster/QuestDataUpdated.h"
#include "PJWrapper.h"
#include "PollingStation.h"
#include "PostMaster/PushState.h"
#include "PostMaster/PopCurrentState.h"

QM::CQuestManager::CQuestManager()
{
	myProgression = 0;
	myObjectives.Init(4);
	myQuests.Init(4);
	myPortalIndexes.Init(4);
	myError = "";
	myLoadSuccess = true;
	POSTMASTER.Subscribe(this, eMessageType::QuestRelated);
}


QM::CQuestManager::~CQuestManager()
{
	POSTMASTER.UnSubscribe(this, eMessageType::QuestRelated);
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
	CompleteEvent();
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

	myProgression += 1;

	myCurrentObjectives = SQuest();

	if (myEvents.Size() < 1)
	{
		SendUpdateMessage();
		return;
	}

	const SEvent nextEvent = myEvents.Pop();

	switch (nextEvent.myType)
	{
	case eEventType::OBJECTIVE:
		myCurrentObjectives.myObjectives.Add(nextEvent.myHandle);
		break;
	case eEventType::QUEST:
		myCurrentObjectives = myQuests[nextEvent.myHandle];
		break;
	case eEventType::OPEN_PORTAL:
		PollingStation::OpenPortals.Add(myPortalIndexes[nextEvent.myHandle]);
		CompleteEvent();
		break;
	case eEventType::CREDITS:
		PostMaster::GetInstance().SendLetter(eMessageType::eStateStackMessage, PushState(PushState::eState::eCreditScreen, 1));
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



void QM::CQuestManager::SendUpdateMessage()
{
	CQuestDataUpdated thisEvent;

	PostMaster::GetInstance().SendLetter(eMessageType::QuestRelated, thisEvent);
}

QM::EventHandle QM::CQuestManager::AddObjective(SObjective anObjective)
{
	const EventHandle handle = myObjectives.Size();
	myObjectiveHandles[anObjective.myName] = handle;
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

QM::SObjective QM::CQuestManager::GetObjective(const int aObjective) const
{
	return myObjectives[aObjective];
}

QM::EventHandle QM::CQuestManager::GetObjectiveHandle(std::string anObjectiveName)
{
	return myObjectiveHandles[anObjectiveName];
}

QM::EventHandle QM::CQuestManager::LoadObjective(const CU::CPJWrapper& anEvent, const std::string& aFileName)
{
	if (anEvent.count("name") < 1)
	{
		myError = "objective is missing a name in file: ";
		myError += aFileName;
		myLoadSuccess = false;
		return false;
	}

	if (anEvent.count("text") < 1)
	{
		myError = "objective is missing a text in file: ";
		myError += aFileName;
		myLoadSuccess = false;
		return false;
	}

	if (anEvent.count("goal") < 1)
	{
		myError = "objective is missing a goal in file: ";
		myError += aFileName;
		myLoadSuccess = false;
		return false;
	}
	
	SObjective objective;
	objective.myName = anEvent.at("name").GetString();
	objective.myText = anEvent.at("text").GetString();
	objective.myGoal = static_cast<short>(anEvent.at("goal").GetUInt());
	objective.myAmmount = 0;

	return AddObjective(objective);
}

QM::EventHandle QM::CQuestManager::LoadQuest(const CU::CPJWrapper& anEvent, const std::string& aFileName)
{
	if (anEvent.count("objectives") < 1)
	{
		myError = "a quest is missing objectives in file : ";
		myError += aFileName;
		myLoadSuccess = false;
		return false;
	}

	SQuest quest;

	for (int i = 0; i < anEvent.at("objectives").Size(); ++i)
	{
		quest.myObjectives.Add(LoadObjective(anEvent.at("objectives")[i], aFileName));
	}

	return AddQuest(quest);
}

bool QM::CQuestManager::LoadQuestlines(std::string aQuestlinesFile)
{
	CU::CPJWrapper jsonDoc;
	std::string errorString;
	if (jsonDoc.Parse(aQuestlinesFile, &errorString) == false)
	{
		myError = errorString;
		myError += " in file: ";
		myError += aQuestlinesFile;
		myLoadSuccess = false;
		return false;
	}

	const CU::JsonObject rootObject = jsonDoc.GetJsonObject();
	if (rootObject.count("questline") < 1)
	{
		myError = "missing \"questline\" object in quest file: ";
		myError += aQuestlinesFile;
		myLoadSuccess = false;
		return false;
	}

	const CU::CPJWrapper questLineObject = rootObject.at("questline");

	if (questLineObject.count("events") < 1 && questLineObject.at("events").IsArray() == false)
	{
		myError = "missing \"events\" array or isn't an array in quest file: ";
		myError += aQuestlinesFile;
		myLoadSuccess = false;
		return false;
	}

	const CU::CPJWrapper eventArray = questLineObject.at("events");
	for (int i = 0; i < eventArray.Size(); ++i)
	{
		if (eventArray[i].IsObject() == false)
		{
			myError = "event array should only contain objects in quest file: ";
			myError += aQuestlinesFile;
			myLoadSuccess = false;
			return false;
		}

		const CU::CPJWrapper currentEvent = eventArray[i];
		if (currentEvent.count("type") < 1 && currentEvent.at("type").IsString() == false)
		{
			myError = "event missing \"type\" in file: ";
			myError += aQuestlinesFile;
			myLoadSuccess = false;
			return false;
		}

		const std::string currentType = currentEvent.at("type").GetString();
		
		if (currentType == "Objective")
		{
			const EventHandle newObjective = LoadObjective(currentEvent, aQuestlinesFile);
			AddEvent(eEventType::OBJECTIVE, newObjective);
			continue;
		}
		if (currentType == "Quest")
		{
			const EventHandle newQuest = LoadQuest(currentEvent, aQuestlinesFile);
			AddEvent(eEventType::QUEST, newQuest);
			continue;
		}
		if (currentType == "QuestLine")
		{
			if (currentEvent.count("questFile") < 1)
			{
				myError = "quest line event missing \"questFile\" in file: ";
				myError += aQuestlinesFile;
				myLoadSuccess = false;
				return false;
			}
			if (LoadQuestlines(currentEvent.at("questFile").GetString()) == false)
			{
				return false;
			}
		}
		if (currentType == "OpenPortal")
		{
			const EventHandle newOpenPortal = myPortalIndexes.Size();

			if (currentEvent.count("portalId") < 1 && !currentEvent.at("portalId").IsNumber())
			{
				myError = "open portal missing portal id";
				myError += aQuestlinesFile;
				myLoadSuccess = false;
				return false;
			}

			myPortalIndexes.Add(currentEvent.at("portalId").GetNumber());
			AddEvent(eEventType::OPEN_PORTAL, newOpenPortal);
			continue;
		}
		if (currentType == "Credits")
		{
			AddEvent(eEventType::CREDITS, 0);
			continue;
		}
	}
	myLoadSuccess = true;
	return true;
}

bool QM::CQuestManager::GetIfLoadingSuceeded() const
{
	return myLoadSuccess;
}
