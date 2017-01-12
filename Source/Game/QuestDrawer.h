#pragma once
#include "../BrontosaurusEngine/TextBox.h"
#include "../PostMaster/Subscriber.h"


namespace QM
{
	struct SQuest;

	class CQuestDrawer: public Subscriber
	{
	public:
		CQuestDrawer();
		~CQuestDrawer();

		void UpdateText();
		void Render();

		eMessageReturn Recieve(const Message& aMessage) override;

	private:
		CTextBox myTextBox;
		CTextInstance* myQuestCompleteText;
	};
}

