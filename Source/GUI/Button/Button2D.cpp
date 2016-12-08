#include "stdafx.h"
#include "Button2D.h"

namespace GUI
{
	Button2D::Button2D(const std::function<void(void)>& aFunction, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible)
		: WidgetContainer(aPosition, aSize, aName, aIsVisible)
		, myAnimationState(eAnimationState::eInActive)
		, myFunction(aFunction)
		, myAnimationTimer(nullptr)
		, myResetPosition(0)
		, myWasClicked(false)
	{
		myAnimationTimer = new CU::Time(0.f);
	}

	Button2D::~Button2D()
	{
		SAFE_DELETE(myAnimationTimer);
	}

	void Button2D::Update(const CU::Time& aDeltaTime)
	{
		const float MilliSecondsPerFrame = 41.7f * .5f;
		CU::Time& animationTimer = *myAnimationTimer;

		if (myAnimationState == eAnimationState::eStarted)
		{
			animationTimer += aDeltaTime;

			if (aDeltaTime.GetMilliseconds() < 60.f)
			{
				//CU::Matrix44f transformation = static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->GetTransformation();
				//float& posY = transformation.GetPosition().y;
				//posY += aDeltaTime.GetMicroseconds() * -0.001f * 0.1f;

				//static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->SetTransformation(transformation);
			}


			if (animationTimer.GetMilliseconds() > MilliSecondsPerFrame * 3.f)
			{
				myAnimationState = eAnimationState::ePaused;
				animationTimer.Reset();
			}
		}
		else if (myAnimationState == eAnimationState::eFlipped)
		{
			animationTimer += aDeltaTime;
			//CU::Matrix44f transformation = static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->GetTransformation();

			//float& posY = transformation.GetPosition().y;
			//posY += aDeltaTime.GetMicroseconds() * 0.001f * 0.1f;
			//static_cast<ModelWidget*>(myDecoratedWidget)->myModelInstance->SetTransformation(transformation);

			//if (posY >= myResetPosition)
			{
				myAnimationState = eAnimationState::eDone;
				animationTimer.Reset();
			}
		}
		else if (myAnimationState == eAnimationState::eDone)
		{
			myAnimationState = eAnimationState::eInActive;
			animationTimer.Reset();

			if (myWasClicked == true)
			{
				RunCallbackFunction();
				myWasClicked = false;
			}
		}
	}

	void Button2D::OnMousePressed(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton)
	{
		//WidgetContainer::OnMousePressed(aMousePosition, aButton);

		if (aButton == CU::eMouseButtons::LBUTTON)
		{
			myAnimationState = eAnimationState::eStarted;
			myWasClicked = true;
		}
	}

	void Button2D::OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton)
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

	//void Button2D::OnMouseMove(const CU::Vector2f& aMousePosition)
	//{
	//}
	//
	//void Button2D::OnMouseEnter(const CU::Vector2f& aMousePosition)
	//{
	//}
	//
	//void Button2D::OnMouseExit(const CU::Vector2f& aMousePosition)
	//{
	//}

	void Button2D::RunCallbackFunction()
	{
		if (myWasClicked == true && myFunction != nullptr)
		{
			myFunction();
		}
	}
}