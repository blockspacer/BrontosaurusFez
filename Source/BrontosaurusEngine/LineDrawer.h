#pragma once

#include <unordered_map>
#include "../CommonUtilities/GrowingArray.h"
#include "../CommonUtilities/vector2.h"
#include "../CommonUtilities/vector4.h"

using LineHandle = int;
using LineIndex = LineHandle;
struct ID3D11Buffer;
class CEffect;

class CLineDrawer
{
	struct SLine2D
	{
		CU::Vector4f myStart;
		CU::Vector4f myEnd;
	};

public:
	CLineDrawer();
	~CLineDrawer();

	void Render();

	LineHandle AddLine(const CU::Vector2f& aStart, const CU::Vector2f& aEnd);
	void AddQuad(const CU::Vector2f& aMinPoint, const CU::Vector2f& aMaxPoint, LineHandle aLineHandlesOut[]);
	void RemoveLine2D(const LineHandle aLineHandle);

private:
	LineHandle AddLine(const SLine2D& aLine2D);

	void CreateVertexBuffers();
	void RenderLines2D();
	void UpdateVertexBuffer2D();

	CU::GrowingArray<SLine2D, LineIndex> myLines2D;
	CU::GrowingArray<LineIndex, LineHandle> myLineHandles2D;

	CEffect* myEffect;
	ID3D11Buffer* myVertexBuffer2D;

	static const LineIndex NullIndex = -2;
	static const unsigned int ourLine2DVertexCount = 2;
	static const unsigned int ourMaxNumberOfLines = USHRT_MAX;
};
