#pragma once
#include "../BrontosaurusEngine/TextBox.h"
#include "../PostMaster/Subscriber.h"
#include "QuestManager.h"


namespace QM
{
	struct SQuest;

	class CQuestDrawer: public Subscriber
	{
	public:
		explicit CQuestDrawer(CQuestManager& aQuestManger);
		~CQuestDrawer();

		void UpdateText();
		void Render();

		eMessageReturn Recieve(const Message& aMessage) override;

	private:
		CTextBox myTextBox;
		CTextInstance* myQuestCompleteText;
		CQuestManager & myQuestManager;
	};
}

