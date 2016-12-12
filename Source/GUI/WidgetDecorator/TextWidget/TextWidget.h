#pragma once
#include <WidgetDecorator/WidgetDecorator.h>
#include "../CommonUtilities/DynamicString.h"

namespace GUI
{
	class TextWidget : public WidgetDecorator
	{
	public:
		TextWidget(Widget* aWidget, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const CU::DynamicString& aText, const bool aIsVisible = true);
		~TextWidget();

	private:
		
	};
}
