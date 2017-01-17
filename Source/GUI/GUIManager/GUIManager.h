#pragma once

#include "../PostMaster/Subscriber.h"

enum class eMessageReturn;

namespace CU
{
	class Time;
	class Camera;
	class DynamicString;
	enum class eMouseButtons;

	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;
}

class CLoaderScene;
class GUICursor;

namespace GUI
{
	class Widget;
	class WidgetContainer;

	class GUIManager : public Subscriber
	{
	public:
		GUIManager();
		~GUIManager();

		void Init();
		void Init(const char* aGUIScenePath);

		void Update(const CU::Time& aDeltaTime);
		void Render();

		void AddWidget(const std::string& aName, Widget* aWidget);
		void DeleteWidget(const std::string& aName);
		Widget* RemoveWidget(const std::string& aName);
		Widget* FindWidget(const std::string& aName);

		void PauseRenderAndUpdate();
		void RestartRenderAndUpdate();

		eMessageReturn MouseClicked(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition);
		eMessageReturn MouseReleased(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition);
		eMessageReturn MouseMoved(const CU::Vector2f& aMousePosition);
		eMessageReturn MouseDragged(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition, const CU::Vector2f& aLastMousePosition);

		eMessageReturn Recieve(const Message& aMessage) override;

		inline void SetRenderMouse(const bool aShouldRenderMouse);

	private:
		WidgetContainer* myWidgetContainer;
		Widget* myFocusedWidget;
		Widget* myWidgetAtMouse;

		GUICursor* myCursor;
		CU::Camera* myCamera;

		bool myShouldUpdate;
		bool myShouldRender;
		bool myShouldStealInput;
		bool myShouldRenderMouse;
	};
}

inline void GUI::GUIManager::SetRenderMouse(const bool aShouldRenderMouse)
{
	myShouldRenderMouse = aShouldRenderMouse;
}
