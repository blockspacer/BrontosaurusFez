#pragma once
#include <map>
#include "../LuaWrapper/SSlua/SSlua.h"
#include "../CommonUtilities/GrowingArray.h"

class CConsole
{
public:
	CConsole();
	~CConsole();

	void Init();
	void GetLuaFunctions();
	void Activate();
	void Deactivate();
	

	void Update();
	void Render();

private:
	const CU::GrowingArray<std::string>& UpdateCommandSuggestions(const std::string& aStringToCompare);




private:
	std::map<std::string, SSlua::LuaCallbackFunction> myLuaFunctions;
	CU::GrowingArray<std::string> mySuggestedCommands;

};

