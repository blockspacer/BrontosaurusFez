#include "stdafx.h"
#include "Subscriber.h"

#include "PostMaster.h"

Subscriber::Subscriber()
{
}

Subscriber::~Subscriber()
{
	PostMaster::GetInstance().UnSubscribeEveryWhere(this);
}
