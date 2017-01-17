#pragma once
#include <WidgetDecorator\WidgetDecorator.h>
//#include <CommonUtilities\vector2.h>
#include "../CommonUtilities/vector2.h"

namespace GUI
{
	class ToolTip : public WidgetDecorator
	{
	public:
		ToolTip(Widget* aDecoratedWidget, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const std::string& aName, const bool aIsVisible = true);
		~ToolTip();

		void OnMouseMove(const CU::Vector2f& aPosition) override;
		void OnMouseEnter(const CU::Vector2f& aMousePosition) override;
		void OnMouseExit(const CU::Vector2f& aMousePosition) override;
		void Render() override;

	private:
		CU::Vector2f myOffsetToWidget;
		bool myShowText;
	};
}
