#include "stdafx.h"
#include "Console.h"
#include "../PostMaster/Event.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PostMaster.h"

CConsole::CConsole()
{
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eKeyPressed);
}


CConsole::~CConsole()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyPressed);
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
	//Börja Renderas
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

eMessageReturn CConsole::TakeKeyBoardInputPressedChar(const char aKey)
{
	int a = 5;
	a++;
	return eMessageReturn::eContinue;
}
