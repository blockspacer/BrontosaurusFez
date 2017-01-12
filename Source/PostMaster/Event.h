#pragma once

#include "EMessageReturn.h"
#include "../Game/QuestManager.h"
#include "../Game/QuestDrawer.h"

namespace GUI
{
	class GUIManager;
}

namespace QM
{
	class CQuestManager;
}

class State;
class PauseMenu;
class GUICursor;
class CPlayState;
class StateStack;
class CreditsState;
class CInputManager;
class CDebugInfoDrawer;
class LevelSelectState;
class EnemyEmitterComponent;
class InputController;
class CConsole;
class CMouseComponent;
class HatShopState;

//temp
class CHealthComponent;
class CRenderer;

class Event
{
public:
	Event() {}
	virtual ~Event() {}

	virtual eMessageReturn DoEvent(State*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(PauseMenu*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(GUICursor*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CPlayState*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(HatShopState*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(StateStack*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CreditsState*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CInputManager*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(GUI::GUIManager*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CDebugInfoDrawer*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(LevelSelectState*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(EnemyEmitterComponent*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(InputController*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CConsole*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CMouseComponent*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(QM::CQuestManager*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(QM::CQuestDrawer *) const { return  eMessageReturn::eContinue; }

	//temp
	virtual eMessageReturn DoEvent(CHealthComponent*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CRenderer*) const { return eMessageReturn::eContinue; }
};
