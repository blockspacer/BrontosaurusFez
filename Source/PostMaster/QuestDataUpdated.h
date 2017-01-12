#pragma once
#include "Event.h"

class CQuestDataUpdated :public Event
{
public:
	CQuestDataUpdated();
	~CQuestDataUpdated();

	eMessageReturn DoEvent(QM::CQuestDrawer *) const override;
};

