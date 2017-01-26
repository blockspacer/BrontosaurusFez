#include "stdafx.h"
#include "HealthWidget.h"
#include "ModelWidget.h"
#include "GUIPixelConstantBuffer.h"

namespace GUI
{
	CHealthWidget::CHealthWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const std::string& aName)
		: WidgetContainer(aPosition, aSize, aName, true)
		, myHealthPercent(1.f)
	{
		PostMaster::GetInstance().Subscribe(this, eMessageType::ePlayerHealthChanged);
	}

	CHealthWidget::~CHealthWidget()
	{
		PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerHealthChanged);
	}
	
	void CHealthWidget::Init()
	{
		ModelWidget* model = *FindWidget("Model");
		if (model != nullptr)
		{
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eIsHealthBar] = 1.f;
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eHealthPercent] = 1.f;
		}
	}

	void CHealthWidget::SetHealthPercent(const float aHealthPercent)
	{
		myHealthPercent = aHealthPercent;

		ModelWidget* model = *FindWidget("Model");
		if (model != nullptr)
		{
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eHealthPercent] = aHealthPercent;
		}
	}

	eMessageReturn CHealthWidget::Recieve(const Message& aMessage)
	{
		return aMessage.myEvent.DoEvent(this);
	}
}
