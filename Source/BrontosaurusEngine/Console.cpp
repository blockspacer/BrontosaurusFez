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
	mySuggestedCommands.Init(myLuaFunctions.count);
	std::map<std::string, SSlua::LuaCallbackFunction>::iterator it;
	for (it = myLuaFunctions.begin(); it != myLuaFunctions.end(); it++)
	{
		resultDifference = YouCanTSpell(aStringToCompare, it->first);
		if (resultDifference < difference)
		{
			difference = resultDifference;
			didYouMean = it->first;
		}
	}
}
