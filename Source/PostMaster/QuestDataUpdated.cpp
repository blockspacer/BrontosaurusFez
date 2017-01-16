#include "stdafx.h"
#include "QuestDataUpdated.h"

#include "../Game/QuestManager.h"
#include "../Game/QuestDrawer.h"

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
