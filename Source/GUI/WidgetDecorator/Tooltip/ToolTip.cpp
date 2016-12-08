#include "stdafx.h"
#include "ToolTip.h"

namespace GUI
{
	ToolTip::ToolTip(Widget* aDecoratedWidget, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible)
		: WidgetDecorator(aDecoratedWidget, aPosition, aSize, aName, aIsVisible), myShowText(false)
	{
	}

	ToolTip::~ToolTip()
	{
	}

	void ToolTip::OnMouseMove(const CU::Vector2f& aPosition)
	{
		WidgetDecorator::OnMouseMove(aPosition);
	}

	void ToolTip::OnMouseEnter(const CU::Vector2f& aMousePosition)
{
		WidgetDecorator::OnMouseEnter(aMousePosition);

		myShowText = true;
	}

	void ToolTip::OnMouseExit(const CU::Vector2f& aMousePosition)
{
		WidgetDecorator::OnMouseExit(aMousePosition);

		myShowText = false;
	}

	void ToolTip::Render()
	{
		WidgetDecorator::Render();

		if (myShowText == true && IsVisible() == true)
		{
		}
	}
}
