#pragma once

#include "../CommonUtilities/vector2.h"

#ifndef SUPRESS_UNUSED_WARNING
#define SUPRESS_UNUSED_WARNING(variable) variable
#endif // !SUPRESS_UNUSED_WARNING

namespace CU
{
	class Time;
	class DynamicString;

	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;

	enum class eMouseButtons;
}

namespace GUI
{
	class ModelWidget;
	class ButtonAnimation;

	class Widget
	{
	public:
		Widget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const bool aIsVisible);
		Widget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible);
		virtual ~Widget();

		virtual void Update(const CU::Time& aDeltaTime) { SUPRESS_UNUSED_WARNING(aDeltaTime); }
		virtual void Render() {}

		virtual void OnMouseMove(const CU::Vector2f& aMousePosition) { SUPRESS_UNUSED_WARNING(aMousePosition); }
		virtual void OnMouseDrag(const CU::Vector2f& aDisplacement, const CU::eMouseButtons aButton) { SUPRESS_UNUSED_WARNING(aDisplacement); SUPRESS_UNUSED_WARNING(aButton); }
		virtual void OnMousePressed(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) { SUPRESS_UNUSED_WARNING(aMousePosition); SUPRESS_UNUSED_WARNING(aButton); }
		virtual void OnMouseDown(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) { SUPRESS_UNUSED_WARNING(aMousePosition); SUPRESS_UNUSED_WARNING(aButton); }
		virtual void OnMouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons aButton) { SUPRESS_UNUSED_WARNING(aMousePosition); SUPRESS_UNUSED_WARNING(aButton); }
		virtual void OnMouseEnter(const CU::Vector2f& aMousePosition) { SUPRESS_UNUSED_WARNING(aMousePosition); }
		virtual void OnMouseExit(const CU::Vector2f& aMousePosition) { SUPRESS_UNUSED_WARNING(aMousePosition); }
		virtual void OnGotFocus() {}
		virtual void OnLostFocus() {}

		virtual Widget* FindWidget(const CU::DynamicString& aWidgetName);
		virtual Widget* MouseIsOver(const CU::Vector2f& aMousePosition);

		virtual void OnPositionChanged() {}

		virtual void SetWorldPosition(const CU::Vector2f& aPosition);
		virtual void SetLocalPosition(const CU::Vector2f& aPosition);
		virtual void SetSize(const CU::Vector2f& aSize);
		virtual void SetVisibility(const bool aVisibility);
		virtual void SetVisibilityRecursive(const bool aVisibility);
		virtual void SetName(const CU::DynamicString& aName);

		virtual CU::Vector2f GetWorldPosition() const;
		virtual const CU::Vector2f& GetLocalPosition() const;
		virtual const CU::Vector2f& GetSize() const;
		virtual bool IsVisible() const;
		virtual bool HasDebugLines() const;
		virtual const CU::DynamicString& GetName() const;

		virtual void RemoveDebugLines() const;

		operator Widget*() { return this; }
		virtual operator ModelWidget*() { return nullptr; }
		virtual operator ButtonAnimation*() { return nullptr; }

	protected:
		void AddDebugLines() const;

		Widget* GetParent();
		const Widget* GetParent() const;

		void SetChild(Widget* aChild);
		void SetParent(Widget* aParent);

		bool IsInside(const CU::Vector2f& aPosition) const;

	private:
		struct WidgetImpl;
		WidgetImpl* myImplementation;

		static int ourNamelessCount;
	};
}
