#pragma once
#include "Component.h"

namespace CU
{
	template<typename T>
	class Vector3;
	using Vector3f = Vector3<float>;
}

class NavigationComponent : public CComponent
{
public:
	NavigationComponent();
	~NavigationComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

private:
	void CalculatePath(CU::Vector2f aTargetPosition);
	CU::GrowingArray<CU::Vector3f> myPath;
};

