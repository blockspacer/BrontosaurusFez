#pragma once
#include "Event.h"

class CUpdateQuestEvent: public Event
{
public:
	CUpdateQuestEvent(std::string aQuestName, const int anAmmount = 1);
	~CUpdateQuestEvent();

	eMessageReturn DoEvent(QM::CQuestManager* aQuestManager) const override;

private:
	const std::string myQuestName;
	const int myAmmount;
};

