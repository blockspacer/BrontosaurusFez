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
	mySuggestedCommands.Init(100);
	myTextLog.Init(20);
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

void CConsole::UpdateCommandSuggestions(const std::string & aStringToCompare)
{
	mySuggestedCommands.RemoveAll();
	std::map<std::string, SSlua::LuaCallbackFunction>::iterator it;
	for (it = myLuaFunctions.begin(); it != myLuaFunctions.end(); it++)
	{
		if (MakeCommandSuggestions(aStringToCompare,it->first) == true)
		{
			mySuggestedCommands.Add(it->first);
		}
	}
}

bool CConsole::MakeCommandSuggestions(const std::string& aStringToCompare, const std::string& aStringToEvaluate)
{
	const size_t m(aStringToCompare.size());
	const size_t n(aStringToEvaluate.size());

	if (m == 0) return false;
	if (n == 0) return false;

	size_t *costs = new size_t[n + 1];

	for (size_t k = 0; k <= n; k++) costs[k] = k;

	size_t i = 0;
	for (std::string::const_iterator it1 = aStringToCompare.begin(); it1 != aStringToCompare.end(); ++it1, ++i)
	{
		costs[0] = i + 1;
		size_t corner = i;

		size_t j = 0;
		for (std::string::const_iterator it2 = aStringToEvaluate.begin(); it2 != aStringToEvaluate.end(); ++it2, ++j)
		{
			size_t upper = costs[j + 1];
			if (*it1 == *it2)
			{
				costs[j + 1] = corner;
			}
			else
			{
				size_t t(upper<corner ? upper : corner);
				costs[j + 1] = (costs[j]<t ? costs[j] : t) + 1;
			}

			corner = upper;
		}
	}

	size_t result = costs[n];
	delete[] costs;
	if (aStringToCompare.size() - aStringToEvaluate.size() == result)
	{
		return true;
	}

	return false;
}

eMessageReturn CConsole::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

eMessageReturn CConsole::TakeKeyBoardInputPressed(const CU::eKeys aKey)
{
	return eMessageReturn::eContinue;
}
