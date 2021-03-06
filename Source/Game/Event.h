#pragma once
#include <string>

namespace QM
{
	enum class eEventType
	{
		OBJECTIVE,
		QUEST,
		QUEST_LINE,
		OPEN_PORTAL,
		DIALOG,
		CREDITS
	};

	typedef int EventHandle;

	struct SEvent
	{
		eEventType myType;
		EventHandle myHandle; 
	};

	struct SObjective
	{
		SObjective():myAmmount(0), myGoal(0){} 
		std::string myName;
		std::string myText;
		short myAmmount;
		short myGoal;
	};

	struct SQuest
	{
		SQuest()
		{
			myObjectives.Init(1);
		}

		CU::GrowingArray<EventHandle> myObjectives;
	};

	struct SQuestLine
	{
		std::string name;
		std::string filePapth;
	};
}
