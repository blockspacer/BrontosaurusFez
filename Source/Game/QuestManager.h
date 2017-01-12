#pragma once
#include "Event.h"
#include "../PostMaster/Subscriber.h"
#include <map>
#include "Queue.h"

namespace QM
{
	class CQuestManager :public Subscriber
	{
	public:
		static void CreateInstance();
		static CQuestManager& GetInstance();
		static void DestroyInstance();

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

		EventHandle GetObjectiveHandle(std::string anObjectiveName);

		bool LoadQuestlines(std::string aQuestline);

		std::string myError;
	private:
		CQuestManager();
		~CQuestManager();

		static void SendUpdateMessage();

		CU::Queue<SEvent, unsigned short> myEvents;
		CU::GrowingArray<SObjective, EventHandle> myObjectives;
		CU::GrowingArray<SQuest, EventHandle> myQuests;

		SQuest myCurrentObjectives;

		std::map<std::string, EventHandle> myObjectiveHandles;

		bool myLoadSuccess;
		static CQuestManager* ourInstance;
	};
	
}
