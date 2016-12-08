#include "stdafx.h"
#include "RenderPackage.h"
#include "Engine.h"
#include "DXFramework.h"
#include <vector2.h>

CRenderPackage::CRenderPackage()
{
	myTexture = nullptr;
	myResource = nullptr;
	myTarget = nullptr;
	myDepth = nullptr;
	myViewport = nullptr;
	myDepthResource = nullptr;

}

CRenderPackage::~CRenderPackage()
{
	if (myTexture != nullptr) myTexture->Release();
	if (myResource != nullptr) myResource->Release();
	if (myTarget != nullptr) myTarget->Release();
	if (myDepth != nullptr) myDepth->Release();
	if (myViewport != nullptr) delete myViewport;
	if (myDepthResource != nullptr) myDepthResource->Release();

}

void CRenderPackage::Init(const CU::Vector2ui & aSize, ID3D11Texture2D * aTexture, DXGI_FORMAT aFormat)
{
	HRESULT result;
	
	myViewport = new D3D11_VIEWPORT;
	myViewport->TopLeftX = 0.f;
	myViewport->TopLeftY = 0.f;
	myViewport->MinDepth = 0.f;
	myViewport->MaxDepth = 1.f;
	myViewport->Width = static_cast<float>(aSize.x);
	myViewport->Height = static_cast<float>(aSize.y);

	myTexture = aTexture;
	if (myTexture == nullptr)
	{
		CreateTexture2D(aSize.x, aSize.y, aFormat);
		result = DEVICE->CreateShaderResourceView(myTexture, nullptr, &myResource);
		CHECK_RESULT(result, "Couldn't create shader resourse view for the RenderPackage");
	}
	


	result = DEVICE->CreateRenderTargetView(myTexture, NULL, &myTarget);
	CHECK_RESULT(result, "Couldn't create render target view for the RenderPackage");
	
	FRAMEWORK->CreateDepthStencil(static_cast<unsigned int>(myViewport->Width), static_cast<unsigned int>(myViewport->Height), myDepth, myDepthResource );

}

void CRenderPackage::Clear()
{
	ID3D11DeviceContext* context = DEVICE_CONTEXT;

	float clearColour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	context->ClearRenderTargetView(myTarget, clearColour);
	context->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void CRenderPackage::Activate()
{
	ID3D11DeviceContext* context = DEVICE_CONTEXT;

	context->OMSetRenderTargets(1, &myTarget, myDepth);
	context->RSSetViewports(1, myViewport);
}

void CRenderPackage::Activate(CRenderPackage& aRenderPackage)
{
	ID3D11DeviceContext* context = DEVICE_CONTEXT;

	D3D11_VIEWPORT viewPorts[2];
	ID3D11RenderTargetView* renderTargets[2];

	viewPorts[0] = *myViewport;
	viewPorts[1] = *aRenderPackage.myViewport;

	renderTargets[0] = myTarget;
	renderTargets[1] = aRenderPackage.myTarget;

	context->OMSetRenderTargets(2, &renderTargets[0], myDepth);
	context->RSSetViewports(2, &viewPorts[0]);

}

ID3D11ShaderResourceView *& CRenderPackage::GetDepthResource()
{
	return myDepthResource;
}

CU::Vector2f CRenderPackage::GetSize()
{
	return CU::Vector2f(myViewport->Width, myViewport->Height);
}

void CRenderPackage::CreateTexture2D(const int aWidth, const int aHeight, DXGI_FORMAT aFormat)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = aWidth;
	textureDesc.Height = aHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = aFormat;
	textureDesc.SampleDesc.Count = 1;  //MSAA coolio   Count = 4;  
	textureDesc.SampleDesc.Quality = 0;//MSAA coolio   Quality = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = DEVICE->CreateTexture2D(&textureDesc, NULL, &myTexture);
	CHECK_RESULT(result, "Failed to create Texture2D for the renderPackage.");
}