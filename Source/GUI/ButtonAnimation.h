#pragma once
#include "WidgetDecorator.h"
#include <functional>

namespace GUI
{
	class ButtonAnimation : public WidgetDecorator
	{
	public:
		friend class Button;
		ButtonAnimation(IWidget* aModelWidget);
		~ButtonAnimation();

		void Update(const CU::Time& aDeltaTime) override;

		void OnMousePressed(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) override;
		void OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) override;

		//void OnMouseMove(const CU::Vector2f& aMousePosition) override;
		void OnMouseEnter(const CU::Vector2f& aMousePosition) override;
		//void OnMouseExit(const CU::Vector2f& aMousePosition) override;

		inline void SetCallbackFunction(const std::function<void(void)>& aCallbackFunction);
		operator ButtonAnimation*() { return this; }

	private:
		void DoStartedAnimation(const CU::Time aDeltaTime);
		void DoFlippedAnimation(const CU::Time aDeltaTime);
		void DoDoneAnimation();

		enum class eAnimationState
		{
			eInActive,
			eStarted,
			eFlipped,
			ePaused,
			eDone

		} myAnimationState;

		std::function<void(void)> myAnimationIsDoneCallback;
		CU::Time* myAnimationTimer;
		float myResetPosition;
	};

	inline void ButtonAnimation::SetCallbackFunction(const std::function<void(void)>& aCallbackFunction)
	{
		assert(myAnimationIsDoneCallback == nullptr && "Should not overwrite animation done callback function");
		myAnimationIsDoneCallback = aCallbackFunction;
	}
}