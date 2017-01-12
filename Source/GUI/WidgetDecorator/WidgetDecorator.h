#pragma once
#include "Widget.h"

namespace CU
{
	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;
}

namespace GUI
{
	class WidgetDecorator : public Widget
	{
	public:
		WidgetDecorator(Widget* aWidget, const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible = true);
		virtual ~WidgetDecorator();

		virtual void Update(const CU::Time& aDeltaTime) override;
		virtual void Render();

		virtual void OnMouseMove(const CU::Vector2f& aMousePosition) override;
		virtual void OnMousePressed(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) override;
		virtual void OnMouseDown(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) override;
		virtual void OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) override;
		virtual void OnMouseEnter(const CU::Vector2f& aMousePosition) override;
		virtual void OnMouseExit(const CU::Vector2f& aMousePosition) override;

		virtual void SetSize(const CU::Vector2f& aSize) override;
		virtual void SetVisibility(const bool aVisibility) override;

		virtual Widget* FindWidget(const CU::DynamicString& aWidgetName) override;
		virtual void OnPositionChanged() override;

		virtual const CU::DynamicString& GetName() const override;
		virtual CU::Vector2f GetWorldPosition() const;
		virtual const CU::Vector2f& GetLocalPosition() const;
		virtual const CU::Vector2f& GetSize() const;
		virtual bool IsVisible() const;

		virtual void OnMouseDrag(const CU::Vector2f& aDisplacement, const CU::eMouseButtons aButton) override;
		virtual void OnGotFocus() override;
		virtual void OnLostFocus() override;
		virtual Widget* MouseIsOver(const CU::Vector2f& aPosition) override;
		virtual void SetWorldPosition(const CU::Vector2f& aPosition) override;
		virtual void SetLocalPosition(const CU::Vector2f& aPosition) override;
		virtual void SetVisibilityRecursive(const bool aVisibility) override;
		virtual void SetName(const CU::DynamicString& aName) override;
		virtual bool HasDebugLines() const override;
		virtual void RemoveDebugLines() const override;

	protected:
		Widget* myDecoratedWidget;
	};
}
