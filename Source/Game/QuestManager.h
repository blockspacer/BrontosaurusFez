#pragma once
#include "Event.h"
#include "../PostMaster/Subscriber.h"
#include <map>
#include "Queue.h"
#include "PJWrapper.h"

namespace QM
{
	class CQuestManager :public Subscriber
	{
	public:
		CQuestManager();
		~CQuestManager();

		void UpdateObjective(EventHandle anObjectiveHandle, int anAmmount = 1);
		bool CheckIfQuestComplete()const;
		void CompleteEvent();

		void AddEvent(const eEventType anEventType, const EventHandle anEventHandle);
		void AddEvent(const SEvent &anEvent);

		SQuest GetCurrentQuest() const;

		eMessageReturn Recieve(const Message& aMessage)override;

		EventHandle AddObjective(SObjective anObjective);
		EventHandle AddQuest(SQuest anObjective);
		SObjective GetObjective(const int aObjective);
		SObjective GetObjective(const int aObjective)const;

		EventHandle GetObjectiveHandle(std::string anObjectiveName);

		EventHandle LoadObjective(const CU::CPJWrapper& anEvent, const std::string& aFileName);
		EventHandle LoadQuest(const CU::CPJWrapper& aCurrentEvent, const std::string& aQuestlinesFile);
		bool LoadQuestlines(std::string aQuestline);
		bool GetIfLoadingSuceeded() const;
		std::string myError;
	private:
		static void SendUpdateMessage();

		CU::Queue<SEvent, unsigned short> myEvents;
		CU::GrowingArray<SObjective, EventHandle> myObjectives;
		CU::GrowingArray<SQuest, EventHandle> myQuests;
		CU::GrowingArray<int, EventHandle> myPortalIndexes;
		CU::GrowingArray<std::string, EventHandle> myDialogs;

		SQuest myCurrentObjectives;

		std::map<std::string, EventHandle> myObjectiveHandles;

		bool myLoadSuccess;
		int myProgression;
	};
	
}
