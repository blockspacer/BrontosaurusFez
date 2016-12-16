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
}

void CConsole::Deactivate()
{
}

eMessageReturn CConsole::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

eMessageReturn CConsole::TakeKeyBoardInputPressed(const CU::eKeys aKey)
{
	return eMessageReturn();
}
