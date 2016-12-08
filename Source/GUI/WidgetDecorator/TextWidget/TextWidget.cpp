#include "stdafx.h"
#include "TextWidget.h"

namespace GUI
{
	TextWidget::TextWidget(Widget* aWidget, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const CU::DynamicString& /*aText*/, const bool aIsVisible)
		: WidgetDecorator(aWidget, aPosition, aSize, aName, aIsVisible)
	{
	}

	TextWidget::~TextWidget()
	{
	}
}
