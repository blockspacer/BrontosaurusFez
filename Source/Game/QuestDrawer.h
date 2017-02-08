#pragma once
#include "../BrontosaurusEngine/TextBox.h"
#include "../PostMaster/Subscriber.h"
#include "QuestManager.h"
#include "../BrontosaurusEngine/TextInstance.h"


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
		CTextInstance myTextInstance;
		CQuestManager & myQuestManager;
	};
}

