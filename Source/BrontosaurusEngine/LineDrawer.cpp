#include "stdafx.h"
#include "LineDrawer.h"

#include "Engine.h"
#include "DXFramework.h"
#include "Effect.h"
#include "EModelBluePrint.h"
#include "ShaderManager.h"

#define LINE_2D_VERTEX_COUNT (myLines2D.Size() * 2)

CLineDrawer::CLineDrawer()
	: myLines2D(8, false)
	, myLineHandles2D(8, false)
	, myVertexBuffer2D(nullptr)
{
	const unsigned int shaderBluePrint = EModelBluePrint::EModelBluePrint_Position;

	CShaderManager* shaderManager = CEngine::GetInstance()->GetShaderManager();
	ID3D11VertexShader* vertexShader = shaderManager->LoadVertexShader(L"Shaders/line_shader.fx", shaderBluePrint);
	ID3D11PixelShader* pixelShader = shaderManager->LoadPixelShader(L"Shaders/line_shader.fx", shaderBluePrint);
	ID3D11InputLayout* inputLayout = shaderManager->LoadInputLayout(L"Shaders/line_shader.fx", shaderBluePrint);

	myEffect = new CEffect(vertexShader, pixelShader, nullptr, inputLayout, D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	CreateVertexBuffers();
}

CLineDrawer::~CLineDrawer()
{
	SAFE_DELETE(myEffect);
}

void CLineDrawer::Render()
{
#ifndef _RETAIL_BUILD
	RenderLines2D();
#endif // !_RETAIL_BUILD
}

LineHandle CLineDrawer::AddLine(const SLine2D& aLine2D)
{
	if (myLines2D.Size() >= ourMaxNumberOfLines)
	{
		DL_ASSERT("To many lines for the system to handle! talk to carl or find out how to fix");
		return static_cast<LineHandle>(-1);
	}

	LineIndex lineIndex = myLines2D.Size();
	myLines2D.Add(aLine2D);

	LineHandle lineHandle = myLineHandles2D.Size();
	myLineHandles2D.Add(lineIndex);

	return lineHandle;
}

LineHandle CLineDrawer::AddLine(const CU::Vector2f& aStart, const CU::Vector2f& aEnd)
{
	SLine2D line2D;
	line2D.myStart.Set(aStart.x, aStart.y, 0.f, 1.f);
	line2D.myEnd.Set(aEnd.x, aEnd.y, 0.f, 1.f);
	return AddLine(line2D);
}

void CLineDrawer::AddQuad(const CU::Vector2f& aMinPoint, const CU::Vector2f& aMaxPoint, LineHandle aLineHandlesOut[])
{
	if (aLineHandlesOut == nullptr)
	{
		DL_ASSERT("aLineHandlesOut cannot be NULL, because then you cannot destroy them!\nExample: Declare \"LineHandle lineHandleQuad[4]\" and send as argument");
		return;
	}

	aLineHandlesOut[0] = AddLine(CU::Vector2f(aMinPoint.x, aMinPoint.y), CU::Vector2f(aMaxPoint.x, aMinPoint.y));
	aLineHandlesOut[1] = AddLine(CU::Vector2f(aMaxPoint.x, aMinPoint.y), CU::Vector2f(aMaxPoint.x, aMaxPoint.y));
	aLineHandlesOut[2] = AddLine(CU::Vector2f(aMaxPoint.x, aMaxPoint.y), CU::Vector2f(aMinPoint.x, aMaxPoint.y));
	aLineHandlesOut[3] = AddLine(CU::Vector2f(aMinPoint.x, aMaxPoint.y), CU::Vector2f(aMinPoint.x, aMinPoint.y));
}

void CLineDrawer::RemoveLine2D(const LineHandle aLineHandle)
{
	LineIndex lineIndex = myLineHandles2D[aLineHandle];
	myLineHandles2D[aLineHandle] = NullIndex;

	myLines2D.RemoveCyclicAtIndex(lineIndex);

	LineIndex movedIndex = myLines2D.Size();
	if (lineIndex != movedIndex)
	{
		LineHandle movedHandle = myLineHandles2D.Find(movedIndex);
		myLineHandles2D[movedHandle] = lineIndex;
	}

	if (movedIndex == 0)
	{
		myLineHandles2D.RemoveAll();
	}
}

void CLineDrawer::CreateVertexBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(SLine2D) * ourMaxNumberOfLines;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	HRESULT result = DEVICE->CreateBuffer(&vertexBufferDesc, nullptr, &myVertexBuffer2D);
	CHECK_RESULT(result, "Failed to create vertex buffer.");
}

void CLineDrawer::RenderLines2D()
{
	if (myVertexBuffer2D != nullptr)
	{
		ID3D11DeviceContext* context = CEngine::GetInstance()->GetFramework()->GetDeviceContext();
		if (context != nullptr)
		{
			myEffect->Activate();

			UpdateVertexBuffer2D();

			UINT stride = sizeof(SLine2D) / 2;
			UINT offset = 0;

			context->IASetVertexBuffers(0, 1, &myVertexBuffer2D, &stride, &offset);
			context->Draw(LINE_2D_VERTEX_COUNT, 0);
		}
	}
}

void CLineDrawer::UpdateVertexBuffer2D()
{
	if (myLines2D.Size() > 0)
	{
		ID3D11DeviceContext* context = CEngine::GetInstance()->GetFramework()->GetDeviceContext();
		if (context != nullptr)
		{
			D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};

			unsigned int bufferSize = sizeof(SLine2D) / 2;
			if (myLines2D.Size() > ourMaxNumberOfLines)
			{
				bufferSize *= ourMaxNumberOfLines * 2;
			}
			else
			{
				bufferSize *= LINE_2D_VERTEX_COUNT;
			}

			context->Map(myVertexBuffer2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			memcpy(mappedSubresource.pData, &myLines2D[0], bufferSize);
			context->Unmap(myVertexBuffer2D, 0);
		}
	}
}
