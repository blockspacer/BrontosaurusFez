#include "stdafx.h"
#include "ToolTipDecorator.h"

#include "../BrontosaurusEngine/TextInstance.h"
#include "RenderableWidgets/ModelWidget/ModelWidget.h"

namespace GUI
{
	CToolTipDecorator::CToolTipDecorator(Widget* aDecoratedWidget, ModelWidget* aBackGround, const std::string& aTooltipText)
		: WidgetDecorator(aDecoratedWidget, CU::Vector2f::Zero, CU::Vector2f::Zero, aDecoratedWidget->GetName() + "_Tooltip", false)
		, myOffsetToMouse(0.f, -0.05f)
		//, myBackGround(aBackGround)
		, myTextInstance(nullptr)
		, myShouldRender(false)
	{
		myTextInstance = new CTextInstance();
		myTextInstance->Init();
		myTextInstance->SetPosition(aDecoratedWidget->GetWorldPosition());
		myTextInstance->SetText(aTooltipText.c_str());

		//myBackGround->SetLocalPosition(aDecoratedWidget->GetWorldPosition());
	}

	CToolTipDecorator::~CToolTipDecorator()
	{
		//SAFE_DELETE(myBackGround);
		SAFE_DELETE(myTextInstance);
	}

	void CToolTipDecorator::OnMouseEnter(const CU::Vector2f& aMousePosition)
	{
		myShouldRender = true;
		//myBackGround->SetLocalPosition(aMousePosition + myOffsetToMouse);
		myTextInstance->SetPosition(aMousePosition + myOffsetToMouse);
	}

	void CToolTipDecorator::OnMouseExit(const CU::Vector2f& aMousePosition)
	{
		myShouldRender = false;
		//myBackGround->SetLocalPosition(aMousePosition + myOffsetToMouse);
		myTextInstance->SetPosition(aMousePosition + myOffsetToMouse);
	}

	GUI::Widget* CToolTipDecorator::MouseIsOver(const CU::Vector2f& aPosition)
	{
		Widget* mouseIsOver = WidgetDecorator::MouseIsOver(aPosition);
		if (mouseIsOver == myDecoratedWidget)
		{
			return this;
		}

		return mouseIsOver;
	}

	void CToolTipDecorator::Render()
	{
		WidgetDecorator::Render();

		if (myShouldRender == true)
		{
			//myBackGround->Render();
			myTextInstance->Render();
		}
	}

	void CToolTipDecorator::OnMouseMove(const CU::Vector2f& aMousePosition)
	{
		//myBackGround->SetLocalPosition(aMousePosition + myOffsetToMouse);
		myTextInstance->SetPosition(aMousePosition + myOffsetToMouse);
	}
}
