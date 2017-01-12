#pragma once

#include "WidgetDecorator/WidgetDecorator.h"

class CTextInstance;

namespace GUI
{
	class CToolTipDecorator : public WidgetDecorator
	{
	public:
		CToolTipDecorator(Widget* aDecoratedWidget, const std::string& aTooltipText);
		~CToolTipDecorator();

		void Render() override;
		void OnMouseMove(const CU::Vector2f& aMousePosition) override;
		void OnMouseEnter(const CU::Vector2f& aMousePosition) override;
		void OnMouseExit(const CU::Vector2f& aMousePosition) override;

		Widget* MouseIsOver(const CU::Vector2f& aPosition) override;

	private:
		CU::Vector2f myOffsetToMouse;
		CTextInstance* myTextInstance;
		bool myShouldRender;
	};
}