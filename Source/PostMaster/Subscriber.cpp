#include "stdafx.h"
#include "Subscriber.h"

#include "PostMaster.h"

Subscriber::Subscriber()
	: myPriority(0)
{
}

Subscriber::~Subscriber()
{
#ifdef _DEBUG
	PostMaster::GetInstance().UnSubscribeEveryWhere(this);
#endif // _DEBUG
}
