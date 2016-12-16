#include "stdafx.h"
#include "Console.h"
#include "../PostMaster/Event.h"
#include "../PostMaster/Message.h"

CConsole::CConsole()
{
}


CConsole::~CConsole()
{
}

void CConsole::Init()
{

}

void CConsole::GetLuaFunctions()
{
	myLuaFunctions = SSlua::LuaWrapper::GetInstance().GetExposedConsoleFunctions();
}

void CConsole::Activate()
{
	//ta all input
	//B�rja Renderas
}

void CConsole::Deactivate()
{
	//sluta ta all input
	//sluta renderas
}

void CConsole::Update()
{
}

void CConsole::Render()
{
}

const CU::GrowingArray<std::string>& CConsole::UpdateCommandSuggestions(const std::string & aStringToCompare)
{
	CU::GrowingArray<std::string> mySuggestedCommands;
	mySuggestedCommands.Init(100);
	std::map<std::string, SSlua::LuaCallbackFunction>::iterator it;
	for (it = myLuaFunctions.begin(); it != myLuaFunctions.end(); it++)
	{
	
	}

	return mySuggestedCommands;
}

eMessageReturn CConsole::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

eMessageReturn CConsole::TakeKeyBoardInputPressed(const CU::eKeys aKey)
{
	return eMessageReturn::eContinue;
}
