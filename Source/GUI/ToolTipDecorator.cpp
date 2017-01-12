#include "stdafx.h"
#include "ToolTipDecorator.h"

#include "../BrontosaurusEngine/TextInstance.h"

namespace GUI
{
	CToolTipDecorator::CToolTipDecorator(Widget* aDecoratedWidget, const std::string& aTooltipText)
		: WidgetDecorator(aDecoratedWidget, CU::Vector2f::Zero, CU::Vector2f::Zero, aDecoratedWidget->GetName() + "_Tooltip", false)
		, myOffsetToMouse(0.f, -0.05f)
		, myTextInstance(nullptr)
		, myShouldRender(false)
	{
		myTextInstance = new CTextInstance();
		myTextInstance->Init();
		myTextInstance->SetPosition(aDecoratedWidget->GetWorldPosition());
		myTextInstance->SetText(aTooltipText.c_str());
	}

	CToolTipDecorator::~CToolTipDecorator()
	{
		SAFE_DELETE(myTextInstance);
	}

	void CToolTipDecorator::OnMouseEnter(const CU::Vector2f& aMousePosition)
	{
		myShouldRender = true;
		myTextInstance->SetPosition(aMousePosition + myOffsetToMouse);
	}

	void CToolTipDecorator::OnMouseExit(const CU::Vector2f& aMousePosition)
	{
		myShouldRender = false;
		myTextInstance->SetPosition(aMousePosition + myOffsetToMouse);
	}

	GUI::Widget* CToolTipDecorator::MouseIsOver(const CU::Vector2f& aPosition)
	{
		if (WidgetDecorator::MouseIsOver(aPosition) == myDecoratedWidget)
		{
			return this;
		}
	}

	void CToolTipDecorator::Render()
	{
		WidgetDecorator::Render();

		if (myShouldRender == true)
		{
			myTextInstance->Render();
		}
	}

	void CToolTipDecorator::OnMouseMove(const CU::Vector2f& aMousePosition)
	{
		myTextInstance->SetPosition(aMousePosition + myOffsetToMouse);
	}
}
