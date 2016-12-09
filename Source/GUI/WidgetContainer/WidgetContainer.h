#pragma once
#include "Widget.h"

//#include <CommonUtilities/GrowingArray.h>
//#include "../CommonUtilities/GrowingArray.h"
#include <unordered_map>
#include <string>

namespace GUI
{
	class WidgetContainer : public Widget
	{
	public:

		WidgetContainer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const bool aIsVisible);
		WidgetContainer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible);
		virtual ~WidgetContainer();

		void AddWidget(const CU::DynamicString& aWidgetName, Widget* aWidget);
		Widget* RemoveWidget(const CU::DynamicString& aWidgetName);
		Widget* FindWidget(const CU::DynamicString& aWidgetName) override;

		virtual void Update(const CU::Time& aDeltaTime) override;
		virtual void Render() override;
		virtual void SetVisibilityRecursive(const bool aVisibility) override;
		virtual void RemoveDebugLines() const override;

		virtual Widget* MouseIsOver(const CU::Vector2f& aPosition) override;

		void MoveToFront(const CU::DynamicString& aWidgetName);

		inline virtual bool IsEmpty() const;
		virtual int SizeOfThis() const override;

	protected:
		std::unordered_map<std::string, Widget*> myWidgets;
		CU::GrowingArray<Widget*> myOrderedWidgets;
	};

	inline bool WidgetContainer::IsEmpty() const
	{
		return myWidgets.empty();
	}

	inline int WidgetContainer::SizeOfThis() const
	{
		int size = sizeof(*this);

		for (unsigned int i = 0; i < myOrderedWidgets.Size(); ++i)
		{
			size += myOrderedWidgets[i]->SizeOfThis();
		}

		return size;
	}
}
