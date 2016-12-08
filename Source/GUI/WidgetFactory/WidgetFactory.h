#pragma once

class CLoaderScene;
class CLoaderCamera;

namespace CU
{
	class Camera;
	class DynamicString;
}

namespace GUI
{
	class Widget;
	class WidgetContainer;
	class ButtonDecorator;

	class WidgetFactory
	{
	public:
		static WidgetContainer* CreateGUIScene(const char* aFilePathFBX, bool isOld, CU::Camera*& aGUIManagerCameraOut);
		static WidgetContainer* CreateGUIScene(const CLoaderScene* aLoaderScene, CU::Camera*& aGUIManagerCameraOut);

		static Widget* CreateButton2D(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aButtonName);

	private:
		static Widget* CreateButton(Widget* aWidget);
		static Widget* CreateHealthBar(Widget* aWidget);
		

		static CU::Camera* ParseCamera(const CLoaderCamera* aCamera);
		static Widget* CreateBoostBar(Widget* aModelWidget);
		static Widget* CreateTimeBar(Widget* aModelWidget);
	};
}
