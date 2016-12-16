#include "stdafx.h"
#include "ButtonAnimation.h"
#include "..\GUI\RenderableWidgets\ModelWidget\ModelWidget.h"
#include "..\BrontosaurusEngine\ModelInstance.h"

namespace GUI
{
	const float MilliSecondsPerFrame = 41.7f * .5f;

	ButtonAnimation::ButtonAnimation(Widget* aModelWidget)
		: WidgetDecorator(aModelWidget, aModelWidget->GetLocalPosition(), aModelWidget->GetSize(), aModelWidget->GetName() + "_ButtonAnimation", aModelWidget->IsVisible())
		, myAnimationIsDoneCallback(nullptr)
		, myAnimationState(eAnimationState::eInActive)
		, myAnimationTimer(nullptr)
		, myResetPosition(0.f)
	{
		myAnimationTimer = new CU::Time(0.f);

		ModelWidget* modelWidget = static_cast<ModelWidget*>(*myDecoratedWidget);
		if (modelWidget != nullptr)
		{
			const CU::Matrix44f& transformation = modelWidget->myModelInstance->GetTransformation();
			myResetPosition = transformation.GetPosition().y;
		}
	}

	ButtonAnimation::~ButtonAnimation()
	{
		SAFE_DELETE(myAnimationTimer);
	}

	void ButtonAnimation::Update(const CU::Time& aDeltaTime)
	{
		if (myAnimationState == eAnimationState::eStarted)
		{
			DoStartedAnimation(aDeltaTime);
		}
		else if (myAnimationState == eAnimationState::eFlipped)
		{
			DoFlippedAnimation(aDeltaTime);
		}
		else if (myAnimationState == eAnimationState::eDone)
		{
			DoDoneAnimation();
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
		ModelWidget* modelWidget = static_cast<ModelWidget*>(*myDecoratedWidget);
		if (modelWidget != nullptr)
		{
			modelWidget->SetFlashTimeToMax();
		}
	}

	void ButtonAnimation::DoStartedAnimation(const CU::Time aDeltaTime)
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

	void ButtonAnimation::DoFlippedAnimation(const CU::Time aDeltaTime)
	{
		*myAnimationTimer += aDeltaTime;
		CU::Matrix44f transformation = static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->GetTransformation();

		float& posY = transformation.GetPosition().y;
		posY += aDeltaTime.GetMicroseconds() * 0.001f * 0.1f;
		static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->SetTransformation(transformation);

		if (posY >= myResetPosition)
		{
			posY = myResetPosition;
			myAnimationState = eAnimationState::eDone;
			myAnimationTimer->Reset();
		}
	}

	void ButtonAnimation::DoDoneAnimation()
	{
		myAnimationState = eAnimationState::eInActive;
		myAnimationTimer->Reset();

		if (myAnimationIsDoneCallback != nullptr)
		{
			myAnimationIsDoneCallback();
		}
	}
}
