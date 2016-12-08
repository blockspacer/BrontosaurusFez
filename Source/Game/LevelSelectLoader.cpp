#include "stdafx.h"
#include "LevelSelectLoader.h"

#include "../GUI/GUIManager/GUIManager.h"
#include "../GUI/WidgetFactory/WidgetFactory.h"

#include "../LuaWrapper/SSlua/SSlua.h"
#include "BrontosaurusEngine/Engine.h"

namespace LevelSelectLoader
{
	void Loadlevels(const char* aLuaFilePath, const char* aTableName, GUI::GUIManager& aGUIManagerOut)
	{
		SSlua::LuaWrapper& luaWrapper = SSlua::LuaWrapper::GetInstance();
		
		std::map<std::string, SSArgument> luaTableMap;
		if (luaWrapper.ParseLuaTable(aLuaFilePath, aTableName, luaTableMap) == true)
		{
			aGUIManagerOut.Init();
			const CU::Vector2f windowSize(CEngine::GetInstance()->GetWindowSize());
			const float startY = windowSize.y * 0.25f;
			const float offsetY = windowSize.y * 0.125f;
			const float posX = windowSize.x * 0.5f;
			const float sizeX = windowSize.x * 0.125f;

			for (unsigned int i = 1; i <= luaTableMap.size(); ++i)
			{
				const std::string LevelString = "level";
				CU::DynamicString levelName = luaTableMap[LevelString + std::to_string(i)].GetString();

				CU::Vector2f position(startY + offsetY * (i - 1), posX);
				CU::Vector2f buttonSize(sizeX, offsetY);

				GUI::Widget* button = GUI::WidgetFactory::CreateButton2D(position, buttonSize, levelName);

				aGUIManagerOut.AddWidget(levelName, button);
			}
		}

	}
}
