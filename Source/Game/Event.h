#pragma once

namespace QM
{
	enum class eEventType
	{
		OBJECTIVE,
		QUEST,
		QUEST_LINE,
	};

	typedef int EventHandle;

	struct SEvent
	{
		eEventType myType;
		EventHandle myHandle; 
	};

	struct SObjective
	{
		std::string myName;
		std::string myText;
		int myAmmount;
		int myGoal;
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
