#pragma once
#include "../BrontosaurusEngine/TextInstance.h"
#include "../PostMaster/Subscriber.h"

class CGameEventMessenger :public Subscriber
{
public:
	CGameEventMessenger();
	~CGameEventMessenger();

	eMessageReturn Recieve(const Message& aMessage) override;

	void Init(const CU::Vector2f &aPosition);
	void Update(const float aDeltaTime);
	void Render()const;

private:
	
	CTextInstance myText;
};

