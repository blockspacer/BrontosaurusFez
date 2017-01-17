#pragma once

namespace Collision
{
	class IRenderCommand;
}

class CCollisionRenderer
{
public:
	CCollisionRenderer();
	~CCollisionRenderer();

	void AddToRender(Collision::IRenderCommand* aRenderMessage);
	const CU::GrowingArray<char, unsigned short, false>& PrepareBuffer();

private:
	CU::GrowingArray<Collision::IRenderCommand*, unsigned short, false> myRenderQueue;
	CU::GrowingArray<char, unsigned short, false> myVertexBuffer;
};
