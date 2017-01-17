#pragma once
#include <string>
struct SShopSelection
{
	typedef unsigned short Cost;
	std::string HatName;
	Cost myCost;

	const bool operator == (SShopSelection aRight)
	{
		for (unsigned int i = 0; i < HatName.length(); ++i)
		{
			if (HatName[i] != aRight.HatName[i])
			{
				return false;
			}
		}
		if (myCost != aRight.myCost)
		{
			return false;
		}
		return true;
	}
	void operator = (SShopSelection aRight)
	{
		HatName = aRight.HatName;
		myCost = aRight.myCost;
	}
};