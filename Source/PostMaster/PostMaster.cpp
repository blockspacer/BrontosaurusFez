#include "stdafx.h"
#include "PostMaster.h"
#include "Subscriber.h"
#include "Message.h"
#include "EMessageReturn.h"

PostMaster* PostMaster::ourInstance = nullptr;

void PostMaster::UnSubscribeEveryWhere(Subscriber* aSubscriber)
{
	for (int messageType = 0; messageType < mySubscribers.Size(); ++messageType)
	{
		for (int subscriber = 0; subscriber < mySubscribers[messageType].Size(); ++subscriber)
		{
			if (mySubscribers[messageType][subscriber] == aSubscriber)
			{
				assert(false && "READ THIS! :) gå till call stacken, kolla vilken destruktor du var i före ~Subscriber(),\n och i den destructorn lägg till en vanlig UnSubscribe på alla meddelanden som den subscribe:ar på. Så slipper vi den här jävla n^2-loopen för varje subscriber som deletas men ligger kvar i post mastern");
				mySubscribers[messageType].RemoveAtIndex(subscriber);
			}
		}
	}
}

void PostMaster::CreateInstance()
{
	if (ourInstance != nullptr)
	{
		assert(!"Post master should not be created twice");
	}
	
	ourInstance = new PostMaster();
}

void PostMaster::DestroyInstance()
{
	if (ourInstance == nullptr)
	{
		assert(!"Post master is not created (is NULL)");
	}

	SAFE_DELETE(ourInstance);
}

PostMaster& PostMaster::GetInstance()
{
	if (ourInstance == nullptr)
	{
		assert(!"Post master is not created (is NULL)");
	}

	return *ourInstance;
}

PostMaster* PostMaster::GetInstancePtr()
{
	return ourInstance;
}

void PostMaster::AppendSubscriber(Subscriber* aSubscriber, const eMessageType aMessageType)
{
	mySubscribers[IntCast(aMessageType)].Add(aSubscriber);
}

void PostMaster::InsertSubscriber(Subscriber* aSubscriber, const eMessageType aMessageType)
{
	mySubscribers[IntCast(aMessageType)].Insert(0, aSubscriber);
}

void PostMaster::UnSubscribe(Subscriber* aSubscriber, const eMessageType aMessageType)
{
	mySubscribers[IntCast(aMessageType)].Remove(aSubscriber);
}

void PostMaster::SendLetter(const Message& aMessage)
{
	for (int i = 0; i < mySubscribers[IntCast(aMessage.myMessageType)].Size(); ++i)
	{
		if (mySubscribers[IntCast(aMessage.myMessageType)][i]->Recieve(aMessage) == eMessageReturn::eStop)
		{
			break;
		}
	}
}

PostMaster::PostMaster()
{
	for (int i = 0; i < mySubscribers.Size(); ++i)
	{
		mySubscribers[i].Init(16);
	}
}

PostMaster::~PostMaster()
{
}
