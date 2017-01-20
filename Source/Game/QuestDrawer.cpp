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
	myTextBox.SetPosition(CU::Vector2f(0.f, 0.2f));

	myQuestCompleteText = new CTextInstance;
	myQuestCompleteText->Init();
	myQuestCompleteText->SetPosition(CU::Vector2f(0.f, 0.8f));
	myQuestCompleteText->SetText("press enter to complete quest :)");

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

		myTextBox.Clear();
		for (int i = 0; i < quest.myObjectives.Size(); ++i)
		{
			const SObjective objective = myQuestManager.GetObjective(quest.myObjectives[i]);
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
	else
	{
		myTextBox.Clear();
		CU::DynamicString error = myQuestManager.myError.c_str();
		myTextBox.AddText(error);
	}
}

void QM::CQuestDrawer::Render()
{
	myTextBox.Render();

	if (myQuestManager.CheckIfQuestComplete())
	{
		myQuestCompleteText->Render();
	}
}

eMessageReturn QM::CQuestDrawer::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
