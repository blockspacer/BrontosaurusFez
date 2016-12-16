#include "stdafx.h"
#include "Console.h"


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
