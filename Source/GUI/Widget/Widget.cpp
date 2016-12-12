#include "stdafx.h"
#include "Widget.h"

#include "../BrontosaurusEngine/Engine.h"
#include "../BrontosaurusEngine/LineDrawer.h"

#ifdef _DEBUG
#define _USE_LINE_DRAWER
#endif // _DEBUG

namespace GUI
{
	int Widget::ourNamelessCount = 0;

	struct Widget::WidgetImpl
	{
		CU::DynamicString myName;
#ifdef _USE_LINE_DRAWER
		LineHandle myLineHandles[4];
#endif // _USE_LINE_DRAWER
		CU::Vector2f myPosition;
		CU::Vector2f mySize;
		Widget* myParent;
		bool myIsVisible;
	};

	Widget::Widget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const bool aIsVisible)
		: Widget(aPosition, aSize, CU::DynamicString("NamelessWidget") + (++ourNamelessCount), aIsVisible)
	{
	}

	Widget::Widget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible)
	{
		myImplementation = new WidgetImpl();
		myImplementation->myName = aName;
		myImplementation->myPosition = aPosition;
		myImplementation->mySize = aSize;
		myImplementation->myParent = nullptr;
		myImplementation->myIsVisible = aIsVisible;

#ifdef _USE_LINE_DRAWER
		memset(myImplementation->myLineHandles, 0, 4);
#endif // _USE_LINE_DRAWER
	}

	Widget::~Widget()
	{
		RemoveDebugLines();
		SAFE_DELETE(myImplementation);
	}

	Widget* Widget::FindWidget(const CU::DynamicString& aWidgetName)
	{
		if (GetName() == aWidgetName)
		{
			return this;
		}

		return nullptr;
	}

	Widget* Widget::MouseIsOver(const CU::Vector2f& aPosition)
	{
		if (IsInside(aPosition) == true)
		{
			return this;
		}

		return nullptr;
	}

	void Widget::SetWorldPosition(const CU::Vector2f& aPosition)
	{
		if (GetParent() == nullptr)
		{
			SetLocalPosition(aPosition);
		}
		else
		{
			SetLocalPosition(aPosition - GetParent()->GetWorldPosition());
		}
	}

	void Widget::SetLocalPosition(const CU::Vector2f& aPosition)
	{
		myImplementation->myPosition = aPosition;
		OnPositionChanged();
	}

	void Widget::SetSize(const CU::Vector2f& aSize)
	{
		myImplementation->mySize = aSize;
	}

	void Widget::SetVisibility(const bool aVisibility)
	{
		myImplementation->myIsVisible = aVisibility;
		
		//if (aVisibility == false)
		//{
		//	RemoveDebugLines();
		//}
	}

	void Widget::SetVisibilityRecursive(const bool aVisibility)
	{
		myImplementation->myIsVisible = aVisibility;
	}

	void Widget::SetName(const CU::DynamicString& aName)
	{
		myImplementation->myName = aName;
	}

	CU::Vector2f Widget::GetWorldPosition() const
	{
		if (GetParent() == nullptr)
		{
			return GetLocalPosition();
		}
		else
		{
			return GetParent()->GetWorldPosition() + GetLocalPosition();
		}
	}

	const CU::Vector2f& Widget::GetLocalPosition() const
	{
		return myImplementation->myPosition;
	}

	const CU::Vector2f& Widget::GetSize() const
	{
		return myImplementation->mySize;
	}

	bool Widget::IsVisible() const
	{
		return myImplementation->myIsVisible;
	}

	bool Widget::HasDebugLines() const
	{
#ifdef _USE_LINE_DRAWER
		const LineHandle NullHandles[] = { 0, 0, 0, 0 };

		if (memcmp(myImplementation->myLineHandles, NullHandles, sizeof(NullHandles)) == 0)
		{
			return false;
		}
#endif // _USE_LINE_DRAWER
		
		return true;
	}

	const CU::DynamicString& Widget::GetName() const
	{
		return myImplementation->myName;
	}

	void Widget::SetParent(Widget* aParent)
	{
		myImplementation->myParent = aParent;
	}

	void Widget::AddDebugLines() const
	{
#ifdef _USE_LINE_DRAWER
		ENGINE->GetLineDrawer().AddQuad(GetWorldPosition(), GetWorldPosition() + GetSize(), myImplementation->myLineHandles);
#endif // _USE_LINE_DRAWER
	}

	void Widget::RemoveDebugLines() const
	{
#ifdef _USE_LINE_DRAWER
		const LineHandle NullHandles[] = { 0, 0, 0, 0 };

		if (memcmp(myImplementation->myLineHandles, NullHandles, sizeof(NullHandles)) != 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				ENGINE->GetLineDrawer().RemoveLine2D(myImplementation->myLineHandles[i]);
			}

			memcpy(myImplementation->myLineHandles, NullHandles, sizeof(NullHandles));
		}
#endif // _USE_LINE_DRAWER
	}

	Widget* Widget::GetParent()
	{
		return myImplementation->myParent;
	}

	const Widget* Widget::GetParent() const
	{
		return myImplementation->myParent;
	}

	void Widget::SetChild(Widget* aChild)
	{
		aChild->SetParent(this);
	}

	bool Widget::IsInside(const CU::Vector2f& aPosition) const
	{
		if (aPosition.x < myImplementation->myPosition.x)
		{
			return false;
		}
		else if (aPosition.y < myImplementation->myPosition.y)
		{
			return false;
		}
		else if (aPosition.x >= myImplementation->myPosition.x + myImplementation->mySize.x)
		{
			return false;
		}
		else if (aPosition.y >= myImplementation->myPosition.y + myImplementation->mySize.y)
		{
			return false;
		}

		return true;
	}
}
