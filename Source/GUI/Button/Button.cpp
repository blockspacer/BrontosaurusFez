#include "stdafx.h"
#include "Button.h"

#include <RenderableWidgets/ModelWidget/ModelWidget.h>
#include "../BrontosaurusEngine/ModelInstance.h"
#include "../Audio/AudioInterface.h"

namespace GUI
{
	ButtonDecorator::ButtonDecorator(const std::function<void(void)>& aFunction, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, Widget* aDecoratedWidget)
		: WidgetDecorator(aDecoratedWidget, aPosition, aSize, aName, true)
		, myAnimationState(eAnimationState::eInActive)
		, myFunction(aFunction)
		, myAnimationTimer(*(new CU::Time(0.f))) //this is weird
		, myWasClicked(false)
	{
	}

	ButtonDecorator::ButtonDecorator(const std::function<void(void)>& aFunction, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, Widget* aDecoratedWidget)
		: ButtonDecorator(aFunction, aPosition, aSize, aDecoratedWidget->GetName() + "_ButtonDecorator", aDecoratedWidget)
	{
	}

	ButtonDecorator::~ButtonDecorator()
	{
		delete &myAnimationTimer;
	}

	void ButtonDecorator::Update(const CU::Time& aDeltaTime)
	{
		WidgetDecorator::Update(aDeltaTime);

		const float MilliSecondsPerFrame = 41.7f * .5f;

		if (myAnimationState == eAnimationState::eStarted)
		{
			myAnimationTimer += aDeltaTime;

			if (aDeltaTime.GetMilliseconds() < 60.f)
			{
				CU::Matrix44f transformation = static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->GetTransformation();
				float& posY = transformation.GetPosition().y;
				posY += aDeltaTime.GetMicroseconds() * -0.001f * 0.1f;

				static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->SetTransformation(transformation);
			}


			if (myAnimationTimer.GetMilliseconds() > MilliSecondsPerFrame * 3.f)
			{
				myAnimationState = eAnimationState::ePaused;
				myAnimationTimer.Reset();
			}
		}
		else if (myAnimationState == eAnimationState::eFlipped)
		{
			myAnimationTimer += aDeltaTime;
			CU::Matrix44f transformation = static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->GetTransformation();

			float& posY = transformation.GetPosition().y;
			posY += aDeltaTime.GetMicroseconds() * 0.001f * 0.1f;
			static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->SetTransformation(transformation);

			if (posY >= myResetPosition)//(myAnimationTimer.GetMilliseconds() > MilliSecondsPerFrame * 3.f)
			{
				myAnimationState = eAnimationState::eDone;
				myAnimationTimer.Reset();
			}
		}
		else if (myAnimationState == eAnimationState::eDone)
		{
			myAnimationState = eAnimationState::eInActive;
			myAnimationTimer.Reset();
			
			if (myWasClicked == true)
			{
				RunCallbackFunction();
				myWasClicked = false;
			}
		}
	}

	void ButtonDecorator::OnMousePressed(const CU::Vector2f& aMousePosition, CU::eMouseButtons aButton)
	{
		WidgetDecorator::OnMousePressed(aMousePosition, aButton);

		if (aButton == CU::eMouseButtons::LBUTTON)
		{
			Audio::CAudioInterface::GetInstance()->PostEvent("ButtonClick");

			myAnimationState = eAnimationState::eStarted;
			myWasClicked = true;
		}
	}

	void ButtonDecorator::OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton)
	{
		if (aButton == CU::eMouseButtons::LBUTTON)
		{
			myAnimationState = eAnimationState::eFlipped;

			if (IsInside(aMousePosition) == false)
			{
				myWasClicked = false;
			}
		}
	}

	void ButtonDecorator::OnMouseMove(const CU::Vector2f& aMousePosition)
	{
		WidgetDecorator::OnMouseMove(aMousePosition);
	}

	void ButtonDecorator::OnMouseEnter(const CU::Vector2f& aMousePosition)
	{
		Audio::CAudioInterface::GetInstance()->PostEvent("ButtonHover");
		static_cast<ModelWidget*>(myDecoratedWidget)->SetFlashTimeToMax();
		WidgetDecorator::OnMouseEnter(aMousePosition);
	}

	void ButtonDecorator::OnMouseExit(const CU::Vector2f& aMousePosition)
	{
		WidgetDecorator::OnMouseExit(aMousePosition);
	}

	void ButtonDecorator::RunCallbackFunction()
	{
		if (myWasClicked == true && myFunction != nullptr)
		{
			myFunction();
		}
	}
}
