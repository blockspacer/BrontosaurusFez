#include "stdafx.h"
#include "ManaWidget.h"
#include "RenderableWidgets\ModelWidget\ModelWidget.h"
#include "GUIPixelConstantBuffer.h"

namespace GUI
{
	CManaWidget::CManaWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const std::string& aName)
		: WidgetContainer(aPosition, aSize, aName, true)
		, myManaPercent(1.f)
	{
		PostMaster::GetInstance().Subscribe(this, eMessageType::ePlayerManaChanged);
	}

	CManaWidget::~CManaWidget()
	{
		PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerManaChanged);
	}

	void CManaWidget::Init()
	{
		ModelWidget* model = *FindWidget("Model");
		if (model != nullptr)
		{
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eIsManaBar] = 1.f;
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eManaPercent] = 1.f;
		}
	}
	
	void CManaWidget::SetManaPercent(const float aManaPercent)
	{
		ModelWidget* model = *FindWidget("Model");
		if (model != nullptr)
		{
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eManaPercent] = aManaPercent;
		}

		myManaPercent = aManaPercent;
	}

	eMessageReturn CManaWidget::Recieve(const Message& aMessage)
	{
		return aMessage.myEvent.DoEvent(this);
	}
}