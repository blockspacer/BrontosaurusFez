#pragma once
#include "Event.h"

namespace CU
{
	enum class eKeys;
}

class KeyDown : public Event
{
public:
	KeyDown(const CU::eKeys& aKey);
	~KeyDown();

private:
	const CU::eKeys& myKey;
};
