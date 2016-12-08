#include "stdafx.h"
#include "WidgetContainer.h"

namespace GUI
{
	WidgetContainer::WidgetContainer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const bool aIsVisible)
		: Widget(aPosition, aSize, aIsVisible), myOrderedWidgets(4)
	{
	}

	WidgetContainer::WidgetContainer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible)
		: Widget(aPosition, aSize, aName, aIsVisible), myOrderedWidgets(4)
	{
	}

	WidgetContainer::~WidgetContainer()
	{
		for (auto it = myWidgets.begin(); it != myWidgets.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}

		myWidgets.clear();
	}

	void WidgetContainer::AddWidget(const CU::DynamicString& aWidgetName, Widget* aWidget)
	{
		assert(myWidgets.find(aWidgetName.c_str()) == myWidgets.end() && "There is already a Widget with that name.");
		myWidgets[aWidgetName.c_str()] = aWidget;
		myOrderedWidgets.Add(aWidget);
		SetChild(aWidget);
	}

	Widget* WidgetContainer::RemoveWidget(const CU::DynamicString& aWidgetName)
	{
		for (auto it = myWidgets.begin(); it != myWidgets.end(); ++it)
		{
			if (aWidgetName == it->first.c_str())
			{
				Widget* removedWidget = it->second;
				myWidgets.erase(it);
				myOrderedWidgets.Remove(it->second);
				return removedWidget;
			}
		}

		return nullptr;
	}

	Widget* WidgetContainer::FindWidget(const CU::DynamicString& aWidgetName)
	{
		auto it = myWidgets.find(aWidgetName.c_str());
		if (it != myWidgets.end())
		{
			return it->second;
		}
		else
		{
			for (it = myWidgets.begin(); it != myWidgets.end(); ++it)
			{
				Widget* widget = it->second->FindWidget(aWidgetName);
				if (widget != nullptr)
				{
					return widget;
				}
			}
		}

		return nullptr;
	}

	void WidgetContainer::Update(const CU::Time& aDeltaTime)
	{
		for (auto it = myWidgets.begin(); it != myWidgets.end(); ++it)
		{
			it->second->Update(aDeltaTime);
		}
	}

	void WidgetContainer::Render()
	{
		if (IsVisible() == true)
		{
			for (unsigned int i = 0; i < myOrderedWidgets.Size(); ++i)
			{
				if (myOrderedWidgets[i]->IsVisible() == true)
				{
					myOrderedWidgets[i]->Render();
				}
			}
		}
	}

	void WidgetContainer::SetVisibilityRecursive(const bool aVisibility)
	{
		for (auto it = myWidgets.begin(); it != myWidgets.end(); ++it)
		{
			it->second->SetVisibility(aVisibility);
		}

		Widget::SetVisibility(aVisibility);
	}

	void WidgetContainer::RemoveDebugLines() const
	{
		for (unsigned int i = 0; i < myOrderedWidgets.Size(); ++i)
		{
			if (myOrderedWidgets[i]->HasDebugLines())
			{
				myOrderedWidgets[i]->RemoveDebugLines();
			}
		}
	}

	Widget* WidgetContainer::MouseIsOver(const CU::Vector2f& aPosition)
	{
		Widget* container = Widget::MouseIsOver(aPosition);
		if (container != nullptr)
		{
			for (auto it = myWidgets.begin(); it != myWidgets.end(); ++it)
			{
				Widget* widget = it->second->MouseIsOver(aPosition);
				if (widget != nullptr)
				{
					return widget;
				}
			}
		}

		return container;
	}

	void WidgetContainer::MoveToFront(const CU::DynamicString& aWidgetName)
	{
		auto it = myWidgets.find(aWidgetName.c_str());
		if (it != myWidgets.end())
		{
			Widget* widgetToMove = it->second;
			myOrderedWidgets.Remove(it->second);
			myOrderedWidgets.Add(widgetToMove);
		}
	}
}
