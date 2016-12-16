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

	class WidgetFactory
	{
	public:
		static WidgetContainer* CreateGUIScene(const char* aFilePathFBX, CU::Camera*& aGUIManagerCameraOut);

	private:
		static WidgetContainer* CreateGUIScene(const CLoaderScene* aLoaderScene, CU::Camera*& aGUIManagerCameraOut);
		static Widget* CreateButton(Widget* aWidget);		

		static CU::Camera* ParseCamera(const CLoaderCamera* aCamera);
	};
}
