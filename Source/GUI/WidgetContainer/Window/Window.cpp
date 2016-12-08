#include "stdafx.h"
#include "Window.h"

#include <DraggableArea\DraggableArea.h>

namespace GUI
{
	Window::Window(const CU::Vector2f& aPosition, const CU::Vector2f& aSize)
		: WidgetContainer(aPosition, aSize, true)
	{
		AddWidget("Dragging bar", new DraggableArea(GetLocalPosition(), CU::Vector2f(GetSize().x, 25.f), this));
	}

	Window::Window(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName)
		: WidgetContainer(aPosition, aSize, aName, true)
	{
	}

	Window::~Window()
	{
	}

	void Window::Init(const CU::DynamicString& /*aBackgroundSpritePath*/, const CU::Vector2f& /*aBackgroundCoords*/)
	{
	}

	void Window::Render()
	{
		WidgetContainer::Render();
	}

	void Window::OnPositionChanged()
	{
	}
}
