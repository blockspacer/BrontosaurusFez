#pragma once
#include <map>
#include "../LuaWrapper/SSlua/SSlua.h"

class CConsole
{
public:
	CConsole();
	~CConsole();

	void Init();
	void GetLuaFunctions();
	void Activate();
	void Deactivate();

private:
	std::map<std::string, SSlua::LuaCallbackFunction> myLuaFunctions;
};

