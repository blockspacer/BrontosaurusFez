#pragma once
#include <string>
#include "ShopStructs.h"
#include "CommonUtilities/GrowingArray.h"
#include "PostMaster/Subscriber.h"


struct SStorage 
{
	CU::GrowingArray<CU::GrowingArray<SShopSelection>> StorageWithBuyOrder;
	CU::GrowingArray<SShopSelection> HatStorage;
};

class CShopStorage : public Subscriber
{
public:
	static void Create();
	static void Destroy();
	static CShopStorage& GetInstance();

	void RemoveHat(const std::string& aHatName);
	void LoadStorage(const std::string& aFilePath);

	inline const SStorage& GetStorage() const;

private:
	struct SMatchReturn
	{
		bool Matched;
		SShopSelection MatchedWithThis;
	};



	const SMatchReturn MatchHatWithName(const std::string& aHatName) const;
	void SetupBuyOrder(const std::string& aFilepath);
	CShopStorage();
	~CShopStorage();
	friend class HatShopState;

	SStorage myStorage;

	static CShopStorage* ourInstance;

	// Inherited via Subscriber
	virtual eMessageReturn Recieve(const Message & aMessage) override;
};

inline const SStorage& CShopStorage::GetStorage() const
{
	return myStorage;
}