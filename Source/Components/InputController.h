#pragma once
#include "Component.h"
#include "../PostMaster/Subscriber.h"

#include "../CommonUtilities/vector2.h"
#include "../BrontosaurusEngine/Navmesh.h"

namespace CU
{
	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;

	class Camera;

	enum class eInputMessage;
	enum class eMouseButtons;
	enum class eKeys;
}


class InputController : public CComponent , public Subscriber
{
public:
	InputController(const CU::Camera& aPlayerCamera);
	~InputController();
	void Update(float aDeltaTime);
	eMessageReturn Recieve(const Message & aMessage) override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	
	void RaycastOnNavmesh();


	eMessageReturn MouseClicked(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition);
	eMessageReturn MouseReleased(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition);
	eMessageReturn MouseMoved(const CU::Vector2f& aMousePosition);
	eMessageReturn TakeInputMessage(const CU::eInputMessage aInputMessage);
	eMessageReturn TakeKeyPressed(const CU::eKeys& aKey);
	eMessageReturn TakeKeyReleased(const CU::eKeys& aKey);
private:
	CU::GrowingArray<CU::eInputMessage> mySkillInputMessageActivators;
	CU::Vector2f myMousePosition;

	SNavmeshTriangle* myTriangle;
	CPath myPath;

	const CU::Camera& myPlayerCamera;
	float myElapsedManaRegenTimer; //#should not be here lol;
	short myManaRecover;
	short mySkillActivatorKeyDown;
	bool myMouseIsDown;
	bool myIsShiftDown;
	bool myIsActive;
	bool myIsRightMouseButtonDown;
	bool myShouldRecoverManaQuickly;
};

