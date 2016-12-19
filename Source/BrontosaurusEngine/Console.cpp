#include "stdafx.h"
#include "Console.h"
#include "../PostMaster/Event.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PostMaster.h"
#include "TextInstance.h"

#include <iostream>

CConsole::CConsole()
{
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eKeyPressed);
	myIsActive = false;
	myHaveIAfterCurrentText = false;
	myElapsedAnimationTimer = 0.0f;
	myAnimationTimerCooldown = 0.5f;
	myCurrentText = new CTextInstance;
	myCurrentText->SetColor(CTextInstance::Red);
	myCurrentText->SetPosition(CU::Vector2f(0.2f,0.8f));
	myCurrentText->SetText("");
	myCurrentText->Init();
}


CConsole::~CConsole()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyPressed);
}

void CConsole::Init()
{
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

void CConsole::Update(float aDeltaTime)
{
	myElapsedAnimationTimer += aDeltaTime;
	if (myAnimationTimerCooldown < myElapsedAnimationTimer)
	{
		myElapsedAnimationTimer = 0.0f;
		if (myHaveIAfterCurrentText == true)
		{
			myCurrentText->SetText(myCurrentText->GetText().SubStr(0, myCurrentText->GetText().Size() - 1));
			
		}
		else
		{
			myCurrentText->SetText(myCurrentText->GetText() + "|");
		}
		myHaveIAfterCurrentText = !myHaveIAfterCurrentText;
	}
}

void CConsole::Render()
{
	if (myIsActive == true)
	{
		myCurrentText->Render();
		for (unsigned short i = 0; i < myTextLog.Size(); i++)
		{
			myTextLog[i]->Render();
		}
	}
}

void CConsole::UpdateCommandSuggestions(const std::string & aStringToCompare)
{
	int finalResultDifferance = 9999;
	int result = 0;

	std::map<std::string, SSlua::LuaCallbackFunction>::iterator it;
	for (it = myLuaFunctions.begin(); it != myLuaFunctions.end(); it++)
	{
		result = MakeCommandSuggestions(aStringToCompare, it->first);
		if (result < finalResultDifferance)
		{
			finalResultDifferance = result;
			mySuggestedCommand = it->first;
		}
	}
}

size_t CConsole::MakeCommandSuggestions(const std::string& aStringToCompare, const std::string& aStringToEvaluate)
{
	const size_t m(aStringToCompare.size());
	const size_t n(aStringToEvaluate.size());

	if (m == 0) return n;
	if (n == 0) return m;

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

	return result;
}

eMessageReturn CConsole::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

eMessageReturn CConsole::TakeKeyBoardInputPressedChar(const char aKey)
{
	if (aKey == '§')
	{
		myIsActive = !myIsActive;
		if (myIsActive == false)
		{
			Deactivate();
		}
		else
		{
			Activate();
		}
		return eMessageReturn::eContinue;
	}
	if (myIsActive == true)
	{
		if (myHaveIAfterCurrentText == true)
		{
			myCurrentText->SetText(myCurrentText->GetText().SubStr(0, myCurrentText->GetText().Size() - 1));
			myHaveIAfterCurrentText = false;
			myElapsedAnimationTimer = myAnimationTimerCooldown;
		}

		if (aKey == '\r')
		{
			myTextLog.Add(new CTextInstance(*myCurrentText));
			for (unsigned short i = 0; i < myTextLog.Size(); i++)
			{
				myTextLog[i]->SetPosition(myTextLog[i]->GetPosition() + CU::Vector2f(0.0f, -0.05f));
			}
			CheckIfTextIsCommand(myCurrentText->GetText());
			myCurrentText->SetText("");
		}
		else if (aKey == '\b')
		{
			myCurrentText->SetText(myCurrentText->GetText().SubStr(0, myCurrentText->GetText().Size() - 1));
		}
		else if (aKey == '\t')
		{
			//Fyll i från den föreslagna funktionen.
		}
		else
		{
			myCurrentText->SetText(myCurrentText->GetText() + aKey);
		}
	}
	return eMessageReturn::eContinue;
}

const CU::DynamicString CConsole::CheckIfTextIsCommand(const CU::DynamicString& aText)
{
	if (aText == "GodMode")
	{
		//doGodMode();
		return aText + " is activated.";
	}
	else if (aText == "help")
	{
		PrintCommands();
	}
	else
	{

		return aText + " was not found perhaps you meant GodMode.";

	}

	return aText + " was not found.";
}

CU::DynamicString CConsole::ParseAndRunFunction(const CU::DynamicString& aString)
{
	
	int currentLetter = aString.FindFirst(' ');
	if (currentLetter ==CU::DynamicString::FoundNone)
	{
		currentLetter = aString.Size();
	}

	const CU::DynamicString commandName = aString.SubStr(0, currentLetter);

	if (myLuaFunctions.count(std::string(commandName.c_str())) == 0)
	{
		return CU::DynamicString("ERROR: Could not find a command with the name -> ") + commandName;
	}

	SSlua::ArgumentList arguments;
	arguments.Init(1);
	while (currentLetter != aString.Size())
	{
		const int beginingOfArg = currentLetter + 1;
		if (aString[beginingOfArg] == '"')
		{
			bool isCorrect = false;
			for (currentLetter += 1; currentLetter < aString.Size(); currentLetter++)
			{
				if (aString[currentLetter] == '"')
				{
					isCorrect = true;
					break;
				}
			}

			if (isCorrect == false)
			{
				return "ERROR: string was not closed! please close it by using \" at the end";
			}

			arguments.Add(SSArgument(aString.SubStr(beginingOfArg + 1, currentLetter - beginingOfArg - 1).c_str()));
		}
		else
		{
			for (currentLetter += 1; currentLetter < aString.Size(); currentLetter++)
			{
				if (aString[currentLetter] == ' ')
				{
					break;
				}
			}

			const CU::DynamicString argumentString = aString.SubStr(beginingOfArg, currentLetter - beginingOfArg);

			if (argumentString.IsBool())
			{
				arguments.Add(SSArgument(argumentString.AsBool()));
			}
			else if (argumentString.IsFloat())
			{
				arguments.Add(SSArgument(argumentString.AsFloat()));
			}
			else
			{
				arguments.Add(SSArgument(argumentString.c_str()));
			}
		}
	}

	const SSlua::ArgumentList returnArguments = myLuaFunctions[commandName.c_str()](arguments);

	CU::DynamicString resultString("");
	if (returnArguments.Size() > 0)
	{
		resultString += "RESULT-> ";

		for (int i = 0; i < returnArguments.Size(); ++i)
		{
			const SSArgument& currentArgument = returnArguments[i];
		}
	}
}
}

void CConsole::Print(const CU::DynamicString & aText)
{
	myCurrentText->SetText(aText);
	myTextLog.Add(new CTextInstance(*myCurrentText));
	CheckIfTextIsCommand(myCurrentText->GetText());
	for (unsigned short i = 0; i < myTextLog.Size(); i++)
	{
		myTextLog[i]->SetPosition(myTextLog[i]->GetPosition() + CU::Vector2f(0.0f, -0.05f));
		std::cout << myTextLog[i]->GetText().c_str() << std::endl;
	}
}

void CConsole::PrintCommands()
{
	std::map<std::string, SSlua::LuaCallbackFunction>::iterator it;
	for (it = myLuaFunctions.begin(); it != myLuaFunctions.end(); it++)
	{
		Print(it->first.c_str());
	}
}