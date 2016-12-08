#pragma once

class Message;
enum class eMessageReturn;

class Subscriber
{
public:
	Subscriber();
	virtual ~Subscriber();

	virtual eMessageReturn Recieve(const Message& aMessage) = 0;
};
