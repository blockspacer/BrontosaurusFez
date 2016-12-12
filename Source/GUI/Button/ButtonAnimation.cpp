#include "stdafx.h"
#include "ButtonAnimation.h"
#include "..\GUI\RenderableWidgets\ModelWidget\ModelWidget.h"
#include "..\BrontosaurusEngine\ModelInstance.h"

namespace GUI
{
	ButtonAnimation::ButtonAnimation(Widget* aModelWidget)
		: WidgetDecorator(aModelWidget, aModelWidget->GetLocalPosition(), aModelWidget->GetSize(), aModelWidget->GetName() + "_ButtonAnimation", aModelWidget->IsVisible())
		, myAnimationIsDoneCallback(nullptr)
		, myAnimationState(eAnimationState::eInActive)
		, myAnimationTimer(nullptr)
		, myResetPosition(0.f)
	{
		myAnimationTimer = new CU::Time(0.f);
	}

	ButtonAnimation::~ButtonAnimation()
	{
		SAFE_DELETE(myAnimationTimer);
	}

	void ButtonAnimation::Update(const CU::Time & aDeltaTime)
	{
		const float MilliSecondsPerFrame = 41.7f * .5f;

		if (myAnimationState == eAnimationState::eStarted)
		{
			*myAnimationTimer += aDeltaTime;

			if (aDeltaTime.GetMilliseconds() < 60.f)
			{
				ModelWidget* modelWidget = static_cast<ModelWidget*>(*myDecoratedWidget);
				if (modelWidget != nullptr)
				{
					CU::Matrix44f transformation = static_cast<GUI::ModelWidget*>(myDecoratedWidget)->myModelInstance->GetTransformation();
					float& posY = transformation.GetPosition().y;
					posY += aDeltaTime.GetMicroseconds() * -0.001f * 0.1f;

					modelWidget->myModelInstance->SetTransformation(transformation);
				}
			}


			if (myAnimationTimer->GetMilliseconds() > MilliSecondsPerFrame * 3.f)
			{
				myAnimationState = eAnimationState::ePaused;
				myAnimationTimer->Reset();
			}
		}
		else if (myAnimationState == eAnimationState::eFlipped)
		{
			*myAnimationTimer += aDeltaTime;
			CU::Matrix44f transformation = static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->GetTransformation();

			float& posY = transformation.GetPosition().y;
			posY += aDeltaTime.GetMicroseconds() * 0.001f * 0.1f;
			static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->SetTransformation(transformation);

			if (posY >= myResetPosition)//(myAnimationTimer.GetMilliseconds() > MilliSecondsPerFrame * 3.f)
			{
				myAnimationState = eAnimationState::eDone;
				myAnimationTimer->Reset();
			}
		}
		else if (myAnimationState == eAnimationState::eDone)
		{
			myAnimationState = eAnimationState::eInActive;
			myAnimationTimer->Reset();

			if (myAnimationIsDoneCallback != nullptr)
			{
				myAnimationIsDoneCallback();
			}

			//if (myWasClicked == true)
			//{
			//	RunCallbackFunction();
			//	myWasClicked = false;
			//}
		}
	}

	void ButtonAnimation::OnMousePressed(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton)
	{
		SUPRESS_UNUSED_WARNING(aMousePosition);
		if (aButton == CU::eMouseButtons::LBUTTON)
		{
			myAnimationState = eAnimationState::eStarted;
		}
	}

	void ButtonAnimation::OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton)
	{
		SUPRESS_UNUSED_WARNING(aMousePosition);
		if (aButton == CU::eMouseButtons::LBUTTON)
		{
			myAnimationState = eAnimationState::eFlipped;
		}
	}
	void ButtonAnimation::OnMouseEnter(const CU::Vector2f& aMousePosition)
	{
		SUPRESS_UNUSED_WARNING(aMousePosition);
		static_cast<ModelWidget*>(myDecoratedWidget)->SetFlashTimeToMax();
	}
}
