#pragma once

namespace GUI
{
	class GUIManager;
}

namespace LevelSelectLoader
{
	void Loadlevels(const char* aLuaFilePath, const char* aTableName, GUI::GUIManager& aGUIManagerOut);
}
