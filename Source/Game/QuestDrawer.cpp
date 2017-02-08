#include "stdafx.h"
#include "QuestDrawer.h"
#include "QuestManager.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"
#include "DynamicString.h"
#include "vector2.h"
#include "PostMaster/PostMaster.h"
#include "TextInstance.h"


QM::CQuestDrawer::CQuestDrawer(CQuestManager& aQuestManger): myQuestManager(aQuestManger)
{
	myTextInstance.Init("Objective");
	myTextInstance.SetPosition(CU::Vector2f(0.f, 0.2f));

	PostMaster::GetInstance().Subscribe(this, eMessageType::QuestRelated);
	UpdateText();
}


QM::CQuestDrawer::~CQuestDrawer()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::QuestRelated);
}

void QM::CQuestDrawer::UpdateText()
{
	if (myQuestManager.GetIfLoadingSuceeded())
	{
		const SQuest quest = myQuestManager.GetCurrentQuest();

		for (int i = 0; i < quest.myObjectives.Size(); ++i)
		{
			const SObjective objective = myQuestManager.GetObjective(quest.myObjectives[i]);
			CU::DynamicString textLine;
			textLine += objective.myText.c_str();
			textLine += " (";
			textLine += objective.myAmmount;
			textLine += "/";
			textLine += objective.myGoal;
			textLine += ")";

			myTextInstance.SetTextLines({ textLine });
		}
	}
	else
	{
		CU::DynamicString error = myQuestManager.myError.c_str();
		myTextInstance.SetTextLines({error});
	}
}

void QM::CQuestDrawer::Render()
{
	myTextInstance.Render();
}

eMessageReturn QM::CQuestDrawer::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
