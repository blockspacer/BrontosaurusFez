#include "stdafx.h"
#include "CollisionRenderMessages.h"
#include "../CommonUtilities/vector4.h"

float WorldGroundY = 0.f;

#include "../Game/PollingStation.h"
#include "../Components/GameObject.h"

Collision::IRenderCommand::IRenderCommand()
{
	if (PollingStation::playerObject != nullptr)
	{
		WorldGroundY = PollingStation::playerObject->GetToWorldTransform().GetPosition().y;
	}
}

Collision::IRenderCommand::~IRenderCommand()
{
}

Collision::CRenderCircle::CRenderCircle(const Intersection::SCircle* aCircle)
	: myCircle(aCircle)
{
}

void Collision::CRenderCircle::DoRenderCall(CU::GrowingArray<char, unsigned short, false>& aVertexBuffer)
{
	const unsigned short NumberOfPoints(16u);
	const float Tau = 3.141592f * 2.f;
	const float Incrementation = Tau / static_cast<float>(NumberOfPoints);

	static CU::GrowingArray<CU::Vector4f, unsigned short, false> circleVertexBuffer(NumberOfPoints);

	for (float angle = 0.f; angle < Tau; angle += Incrementation)
	{
		CU::Vector4f pointOnCircle(myCircle->myCenterPosition.x, WorldGroundY, myCircle->myCenterPosition.y, 1.f);

		pointOnCircle.x += myCircle->myRadius * std::cosf(angle);
		pointOnCircle.z += myCircle->myRadius * std::sinf(angle);

		circleVertexBuffer.Add(pointOnCircle);
	}

	aVertexBuffer.AddChunk(circleVertexBuffer.AsVoidPointer(), circleVertexBuffer.ByteSize());
	circleVertexBuffer.RemoveAll();
}

Collision::CRenderTriangle::CRenderTriangle(const Intersection::STriangle* aTriangle)
	: myTriangle(aTriangle)
{
}

void Collision::CRenderTriangle::DoRenderCall(CU::GrowingArray<char, unsigned short, false>& aVertexBuffer)
{
	CU::Vector4f triangleVertexBuffer[6u] =
	{
		CU::Vector4f(myTriangle->myPoints[0].x, WorldGroundY, myTriangle->myPoints[0].y, 1.f), CU::Vector4f(myTriangle->myPoints[1].x, WorldGroundY, myTriangle->myPoints[1].y, 1.f),
		CU::Vector4f(myTriangle->myPoints[1].x, WorldGroundY, myTriangle->myPoints[1].y, 1.f), CU::Vector4f(myTriangle->myPoints[2].x, WorldGroundY, myTriangle->myPoints[2].y, 1.f),
		CU::Vector4f(myTriangle->myPoints[2].x, WorldGroundY, myTriangle->myPoints[2].y, 1.f), CU::Vector4f(myTriangle->myPoints[0].x, WorldGroundY, myTriangle->myPoints[0].y, 1.f)
	};

	aVertexBuffer.AddChunk(triangleVertexBuffer, sizeof(triangleVertexBuffer));
}
