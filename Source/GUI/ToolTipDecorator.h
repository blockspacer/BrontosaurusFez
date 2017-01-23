#pragma once

#include "WidgetDecorator/WidgetDecorator.h"

class CTextInstance;

namespace GUI
{
	class ModelWidget;
	class CToolTipDecorator : public WidgetDecorator
	{
	public:
		CToolTipDecorator(Widget* aDecoratedWidget, ModelWidget* aBackGround, const std::string* const aTooltipText, const std::function<bool(std::string&)>& aGetTextFunction);
		~CToolTipDecorator();

		void Render() override;
		void OnMouseMove(const CU::Vector2f& aMousePosition) override;
		void OnMouseEnter(const CU::Vector2f& aMousePosition) override;
		void OnMouseExit(const CU::Vector2f& aMousePosition) override;

		Widget* MouseIsOver(const CU::Vector2f& aPosition) override;

	private:
		std::function<bool(std::string&)> myGetTextFunction;

		CU::Vector2f myOffsetToMouse;
		//ModelWidget* myBackGround;
		CTextInstance* myTextInstance;
		bool myShouldRender;
	};
}