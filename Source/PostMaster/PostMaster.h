#pragma once

#include "MessageType.h"

//#include "../CommonUtilities/StaticArray.h"
//#include "../CommonUtilities/GrowingArray.h"

#define POSTMASTER PostMaster::GetInstance()

class Subscriber;
class Message;

class PostMaster
{
public:
	static void CreateInstance();
	static void DestroyInstance();
	static PostMaster& GetInstance();
	static PostMaster* GetInstancePtr();

	void AppendSubscriber(Subscriber* aSubscriber, const eMessageType aMessageType);
	void InsertSubscriber(Subscriber* aSubscriber, const eMessageType aMessageType);
	void UnSubscribe(Subscriber* aSubscriber, const eMessageType aMessageType);

	void SendLetter(const Message& aMessage);

private:
	PostMaster();
	~PostMaster();

	CU::StaticArray<CU::GrowingArray<Subscriber*, int, false>, MessageTypeLength> mySubscribers;

	static PostMaster* ourInstance;
public:
	void UnSubscribeEveryWhere(Subscriber* aSubscriber);
};

