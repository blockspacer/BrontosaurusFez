#pragma once

#include "EMessageReturn.h"

namespace GUI
{
	class GUIManager;
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
class ControllerComponent;
class EnemyEmitterComponent;
class InputController;

class Event
{
public:
	Event() {}
	virtual ~Event() {}

	virtual eMessageReturn DoEvent(State*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(PauseMenu*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(GUICursor*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CPlayState*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(StateStack*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CreditsState*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CInputManager*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(GUI::GUIManager*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(CDebugInfoDrawer*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(LevelSelectState*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(EnemyEmitterComponent*) const { return eMessageReturn::eContinue; }
	virtual eMessageReturn DoEvent(InputController*) const { return eMessageReturn::eContinue; }
};
