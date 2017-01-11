#include "stdafx.h"
#include "QuestDrawer.h"
#include "QuestManager.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"
#include "DynamicString.h"
#include "vector2.h"
#include "PostMaster/PostMaster.h"


QM::CQuestDrawer::CQuestDrawer()
{
	myTextBox.SetPosition(CU::Vector2f(0.f, 0.2f));
	PostMaster::GetInstance().Subscribe(this, eMessageType::QuestRelated);
}


QM::CQuestDrawer::~CQuestDrawer()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::QuestRelated);
}

void QM::CQuestDrawer::UpdateText()
{
	const SQuest quest = CQuestManager::GetInstance().GetCurrentQuest();

	myTextBox.Clear();
	for (int i = 0; i < quest.myObjectives.Size(); ++i)
	{
		const SObjective objective = CQuestManager::GetInstance().GetObjective(quest.myObjectives[i]);
		CU::DynamicString textLine1;
		textLine1 += objective.myName.c_str();
		textLine1 += ": ";
		CU::DynamicString textLine2;
		textLine2 += objective.myText.c_str();
		textLine2 += " (";
		textLine2 += objective.myAmmount;
		textLine2 += "/";
		textLine2 += objective.myGoal;
		textLine2 += ")";

		myTextBox.AddText(textLine1);
		myTextBox.NewLine();
		myTextBox.AddText(textLine2);
		myTextBox.NewLine();
	}
}

void QM::CQuestDrawer::Render()
{
	myTextBox.Render();
}

eMessageReturn QM::CQuestDrawer::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
