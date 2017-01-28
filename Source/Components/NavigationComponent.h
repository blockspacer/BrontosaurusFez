#pragma once
#include "Component.h"
#include "../BrontosaurusEngine/Navmesh.h"
#include "Synchronizer.h"

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
	void CalculatePath(const CU::Vector2f& aTargetPosition);
	void PathfindOnNavmesh();
	void ThreadedPathfind();

private:
	CPath myPath;
	CPath mySecondPath;
	
	CU::Vector2f myTargetPosition;
	bool myThreadHasFinished;
};

