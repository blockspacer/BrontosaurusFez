#include "stdafx.h"
#include "QuestDataUpdated.h"


CQuestDataUpdated::CQuestDataUpdated()
{
}


CQuestDataUpdated::~CQuestDataUpdated()
{
}

eMessageReturn CQuestDataUpdated::DoEvent(QM::CQuestDrawer* aQuestDrawer) const
{
	aQuestDrawer->UpdateText();
	return eMessageReturn::eContinue;
}
