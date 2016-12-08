#pragma once
#include <WidgetDecorator/WidgetDecorator.h>
#include <functional>

namespace GUI
{
	class ButtonDecorator : public WidgetDecorator
	{
	public:
		ButtonDecorator(const std::function<void(void)>& aFunction, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, Widget* aDecoratedWidget);
		ButtonDecorator(const std::function<void(void)>& aFunction, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, Widget* aDecoratedWidget);
		~ButtonDecorator();

		void Update(const CU::Time& aDeltaTime) override;

		void OnMousePressed(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) override;
		void OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) override;

		void OnMouseMove(const CU::Vector2f& aMousePosition) override;
		void OnMouseEnter(const CU::Vector2f& aMousePosition) override;
		void OnMouseExit(const CU::Vector2f& aMousePosition) override;

		AUTO_IMPLEMENT_SIZE

	private:
		void RunCallbackFunction();

		enum class eAnimationState
		{
			eInActive,
			eStarted,
			eFlipped,
			ePaused,
			eDone

		} myAnimationState;

		std::function<void(void)> myFunction;
		CU::Time& myAnimationTimer;
		float myResetPosition;
		bool myWasClicked;
	};
}
