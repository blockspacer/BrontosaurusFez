#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"
namespace CU
{
	enum class eMouseButtons;
}
class InputController : public CComponent , public Subscriber
{
public:
	InputController();
	~InputController();
	void Update(float aDeltaTime);
	eMessageReturn Recieve(const Message & aMessage) override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

	eMessageReturn MouseClicked(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition);
private:

};

