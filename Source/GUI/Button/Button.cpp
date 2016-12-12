#include "stdafx.h"
#include "Button.h"

#include <RenderableWidgets/ModelWidget/ModelWidget.h>
#include "../BrontosaurusEngine/ModelInstance.h"
#include "../Audio/AudioInterface.h"
#include "ButtonAnimation.h"

namespace GUI
{
	Button::Button(const std::function<void(void)>& aFunction, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName)
		: WidgetContainer(aPosition, aSize, aName, true)
		//, myAnimationState(eAnimationState::eInActive)
		, myFunction(aFunction)
		//, myAnimationTimer(*(new CU::Time(0.f))) //this is weird
		, myWasClicked(false)
	{
	}

	Button::~Button()
	{
		//delete &myAnimationTimer;
	}

	void Button::AddWidget(const CU::DynamicString& aWidgetName, Widget* aWidget)
	{
		WidgetContainer::AddWidget(aWidgetName, aWidget);

		if (aWidgetName == "Animation")
		{
			auto onAnimationDone = [this]
			{
				if (myWasClicked == true)
				{
					RunCallbackFunction();
					myWasClicked = false;
				}
			};

			static_cast<ButtonAnimation*>(aWidget)->SetCallbackFunction(onAnimationDone);
		}
	}

	Widget* Button::MouseIsOver(const CU::Vector2f & aPosition)
	{
		return Widget::MouseIsOver(aPosition);
	}

	void Button::Update(const CU::Time& aDeltaTime)
	{
		WidgetContainer::Update(aDeltaTime);

		//const float MilliSecondsPerFrame = 41.7f * .5f;

		//if (myAnimationState == eAnimationState::eStarted)
		//{
		//	//myAnimationTimer += aDeltaTime;

		//	//if (aDeltaTime.GetMilliseconds() < 60.f)
		//	//{

		//	//	CU::Matrix44f transformation = static_cast<ModelWidget*>(static_cast<ButtonAnimation*>(FindWidget("Animation"))->myDecoratedWidget)->myModelInstance->GetTransformation();
		//	//	float& posY = transformation.GetPosition().y;
		//	//	posY += aDeltaTime.GetMicroseconds() * -0.001f * 0.1f;

		//	//	static_cast<ModelWidget*>(FindWidget("Animation"))->myModelInstance->SetTransformation(transformation);
		//	//}


		//	//if (myAnimationTimer.GetMilliseconds() > MilliSecondsPerFrame * 3.f)
		//	//{
		//	//	myAnimationState = eAnimationState::ePaused;
		//	//	myAnimationTimer.Reset();
		//	//}
		//}
		//else if (myAnimationState == eAnimationState::eFlipped)
		//{
		//	//myAnimationTimer += aDeltaTime;
		//	//CU::Matrix44f transformation = static_cast<ModelWidget*>(FindWidget("Animation"))->myModelInstance->GetTransformation();

		//	//float& posY = transformation.GetPosition().y;
		//	//posY += aDeltaTime.GetMicroseconds() * 0.001f * 0.1f;
		//	//static_cast<ModelWidget*>(FindWidget("Animation"))->myModelInstance->SetTransformation(transformation);

		//	//if (posY >= myResetPosition)//(myAnimationTimer.GetMilliseconds() > MilliSecondsPerFrame * 3.f)
		//	//{
		//	//	myAnimationState = eAnimationState::eDone;
		//	//	myAnimationTimer.Reset();
		//	//}
		//}
		//else if (myAnimationState == eAnimationState::eDone)
		//{
		//	//myAnimationState = eAnimationState::eInActive;
		//	//myAnimationTimer.Reset();
		//	
		//	//if (myWasClicked == true)
		//	//{
		//	//	RunCallbackFunction();
		//	//	myWasClicked = false;
		//	//}
		//}
	}

	void Button::OnMousePressed(const CU::Vector2f& aMousePosition, CU::eMouseButtons aButton)
	{
		//WidgetContainer::OnMousePressed(aMousePosition, aButton);
		for (Widget* child : myOrderedWidgets)
		{
			child->OnMousePressed(aMousePosition, aButton);
		}

		if (aButton == CU::eMouseButtons::LBUTTON)
		{
			Audio::CAudioInterface::GetInstance()->PostEvent("ButtonClick");

			//myAnimationState = eAnimationState::eStarted;
			myWasClicked = true;
		}
	}

	void Button::OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton)
	{
		//WidgetContainer::OnMouseReleased(aMousePosition, aButton);
		for (Widget* child : myOrderedWidgets)
		{
			child->OnMouseReleased(aMousePosition, aButton);
		}

		if (aButton == CU::eMouseButtons::LBUTTON)
		{
			//myAnimationState = eAnimationState::eFlipped;

			if (IsInside(aMousePosition) == false)
			{
				myWasClicked = false;
			}
		}
	}

	void Button::OnMouseMove(const CU::Vector2f& aMousePosition)
	{
		WidgetContainer::OnMouseMove(aMousePosition);
	}

	void Button::OnMouseEnter(const CU::Vector2f& aMousePosition)
	{
		Audio::CAudioInterface::GetInstance()->PostEvent("ButtonHover");
		//static_cast<ModelWidget*>(FindWidget("Animation"))->SetFlashTimeToMax();
		WidgetContainer::OnMouseEnter(aMousePosition);
	}

	void Button::OnMouseExit(const CU::Vector2f& aMousePosition)
	{
		WidgetContainer::OnMouseExit(aMousePosition);
	}

	void Button::RunCallbackFunction()
	{
		if (myWasClicked == true && myFunction != nullptr)
		{
			myFunction();
		}
	}
}
