#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"
#include "../CommonUtilities/vector2.h"
#include "../CommonUtilities/vector3.h"
namespace CU
{
	class Camera;
}

class CClickPulse;

class CMouseComponent : public CComponent, public Subscriber
{
public:
	CMouseComponent(const CU::Camera& aPlayerCamera);
	~CMouseComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

	void MouseMoved(const CU::Vector2f& aMousePosition);
	void Update();
	void SetMouseIsDown(const bool aIsDown);
	void CheckIfHoveredGameObjectDied(CGameObject* aGameobjectThatDied);
	void RemoveHoveredObjects();
private:
	void HighlightClosest();
	void HandleCollision(CGameObject* aCollidedWith);
	eMessageReturn Recieve(const Message& aMessage) override;

	CU::GrowingArray<CGameObject*> myHoveredGameObjects;
	CU::Vector3f myMousePosition;
	bool myMouseIsDown;
	CClickPulse* myClickPulse;
	const CU::Camera& myPlayerCamera;
};
