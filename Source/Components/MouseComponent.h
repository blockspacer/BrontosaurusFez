#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"

namespace CU
{
	class Camera;
}

class CMouseComponent : public CComponent, public Subscriber
{
public:
	CMouseComponent(const CU::Camera& aPlayerCamera);
	~CMouseComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

	void MouseMoved(const CU::Vector2f& aMousePosition);

private:
	eMessageReturn Recieve(const Message& aMessage) override;

	const CU::Camera& myPlayerCamera;
};
