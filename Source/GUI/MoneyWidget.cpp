#include "stdafx.h"
#include "MoneyWidget.h"
#include "ModelWidget.h"
#include "GUIPixelConstantBuffer.h"

#include "../Game/PollingStation.h"
#include "../Components/PlayerData.h"
#include <intsafe.h>
#include "Game\ShopStorage.h"

namespace GUI
{
	CMoneyWidget::CMoneyWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const std::string& aName)
		: WidgetContainer(aPosition, aSize, aName, true)
		//, myMoneyPercent(0.25f)
		, myMoney(PollingStation::playerData->myGold)
	{
		//PostMaster::GetInstance().Subscribe(this, eMessageType::ePlayerMoneyChanged);
	}

	CMoneyWidget::~CMoneyWidget()
	{
		//PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerMoneyChanged);
	}

	void CMoneyWidget::Init()
	{
		ModelWidget* model = *FindWidget("Model");
		if (model != nullptr)
		{
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eIsMoneyBar] = 1.f;
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eMoneyPercent] = 0.f;
		}
	}

	void CMoneyWidget::Update(const CU::Time& aDeltaTime)
	{
		SUPRESS_UNUSED_WARNING(aDeltaTime);

		short cheapestHatPrice = GetCheapestHatPrice();
		if (cheapestHatPrice == 0)
		{
			cheapestHatPrice = myMoney;
		}

		float moneyPercent = static_cast<float>(myMoney) / static_cast<float>(cheapestHatPrice);

#define min(a,b) (((a) < (b)) ? (a) : (b))
		SetMoneyPercent(min(moneyPercent, 1.f));
#undef min
	}

	void CMoneyWidget::SetMoneyPercent(const float aMoneyPercent)
	{
		ModelWidget* model = *FindWidget("Model");
		if (model != nullptr)
		{
			model->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eMember::eMoneyPercent] = aMoneyPercent;
		}

		//myMoneyPercent = aMoneyPercent;
	}

	eMessageReturn CMoneyWidget::Recieve(const Message& /*aMessage*/)
	{
		assert(!"not implemented post mastering for money widget");
		//return aMessage.myEvent.DoEvent(this);
		return eMessageReturn::eContinue;
	}

	short CMoneyWidget::GetCheapestHatPrice()
	{
		unsigned short lowestCost = USHRT_MAX;

		const CU::GrowingArray<CU::GrowingArray<SShopSelection>>& storageWithBuyOrder = CShopStorage::GetInstance().GetStorage().StorageWithBuyOrder;
		for (unsigned int i = 0; i < storageWithBuyOrder.Size(); ++i)
		{
			if (storageWithBuyOrder[i].Size() == 0) continue;

			if (storageWithBuyOrder[i][0].myCost < lowestCost)
			{
				lowestCost = storageWithBuyOrder[i][0].myCost;
			}
		}

		return static_cast<short>(lowestCost);
	}
}
