#include "stdafx.h"
#include "DXFramework.h"

CDXFramework::CDXFramework()
{
	mySwapchain					= nullptr;
	myDevice					= nullptr;
	myDeviceContext				= nullptr;
	myRenderTargetView			= nullptr;
	myDepthStencilView			= nullptr;

}

CDXFramework::~CDXFramework()
{
	Shutdown();
}

bool CDXFramework::Initialize(const int aWidth, const int aHeight, const bool aIsFullScreen, HWND aHWND)
{
	HRESULT result;
	
	//Create device and swapchain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = aWidth;
	swapChainDesc.BufferDesc.Height = aHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = aHWND;
	swapChainDesc.SampleDesc.Count = 1; // MSAA * 4 asball?
	swapChainDesc.SampleDesc.Quality = 0; // 
	swapChainDesc.Windowed = !aIsFullScreen;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create Devive and Swap chain.

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined (DEBUG) || defined (_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &mySwapchain, &myDevice, NULL, &myDeviceContext);
	CHECK_RESULT(result, "Failed to create device & swapchain.");

	// Create Render-target-view
	ID3D11Texture2D* backBuffer = nullptr;	
	result = mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer); 
	CHECK_RESULT(result, "Failed to get Backbuffer.");

	ID3D11ShaderResourceView* test = nullptr;
	CreateDepthStencil(aWidth, aHeight, myDepthStencilView, test);
	test->Release();


	result = myDevice->CreateRenderTargetView(backBuffer, NULL, &myRenderTargetView); //instead of null we can pass a D3D11_RENDERTARGETVIEW_DESC.
	backBuffer->Release();
	CHECK_RESULT(result, "Failed to create Render Target View.");
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	myDeviceContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthStencilView);

	// Create Viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = (FLOAT)aWidth;
	viewPort.Height = (FLOAT)aHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	myDeviceContext->RSSetViewports(1, &viewPort);
	
	return true;
}

void CDXFramework::DisableDepthStencil()
{
	myDeviceContext->OMSetRenderTargets(1, &myRenderTargetView, NULL);
}

void CDXFramework::EnableDepthStencil()
{
	myDeviceContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthStencilView);
}

void CDXFramework::ClearScreen()
{
	float clearColour[4] = { 0.f, 0.f, 0.0f, 0.f };
	myDeviceContext->ClearRenderTargetView(myRenderTargetView, clearColour);
}

void CDXFramework::ClearDepthStencil()
{
	// Clear the depth buffer.
	myDeviceContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void CDXFramework::Resize(const unsigned int aWidth, const unsigned int aHeight) //fix this >:3
{
	myRenderTargetView->Release();
	myRenderTargetView = nullptr;

	HRESULT result;

	result = mySwapchain->ResizeBuffers(0, aWidth, aHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	CHECK_RESULT(result, "Couldn't Resize buffer");

	ID3D11Texture2D* backBufferPtr;

	result = mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	CHECK_RESULT(result, "Couldn't get backbuffer");

	result = myDevice->CreateRenderTargetView(backBufferPtr, NULL, &myRenderTargetView);
	CHECK_RESULT(result, "Couldn't create render target view");

	ID3D11ShaderResourceView* test = nullptr;
	CreateDepthStencil(aWidth, aHeight, myDepthStencilView, test);
	test->Release();


	backBufferPtr->Release();
	backBufferPtr = nullptr;
	myDeviceContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthStencilView);

	SetViewPort(aWidth, aHeight, 0, 1, 0, 0);
}

void CDXFramework::SetViewPort(const unsigned int aWidth, const unsigned int aHeight, const float aMinDepth, const float aMaxDepth, const float aTopLeftX, const float aTopLeftY)
{
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(aWidth);
	viewport.Height = static_cast<float>(aHeight);
	viewport.MinDepth = aMinDepth;
	viewport.MaxDepth = aMaxDepth;
	viewport.TopLeftX = aTopLeftX;
	viewport.TopLeftY = aTopLeftY;
	myDeviceContext->RSSetViewports(1, &viewport);
}

void CDXFramework::Render()
{
	mySwapchain->Present(0, 0);
}

void CDXFramework::Shutdown()
{
	mySwapchain->SetFullscreenState(false, nullptr);

	if (mySwapchain != nullptr)
	{
		mySwapchain->Release();
		mySwapchain = nullptr;
	}
	if (myDevice != nullptr)
	{
		myDevice->Release();
		myDevice = nullptr;
	}
	if (myDeviceContext != nullptr)
	{
		myDeviceContext->Release();
		myDeviceContext = nullptr;
	}
	if (myRenderTargetView != nullptr)
	{
		myRenderTargetView->Release();
		myRenderTargetView = nullptr;
	}

	if (myDepthStencilView != nullptr)
	{
		myDepthStencilView->Release();
		myDepthStencilView = nullptr;
	}
}

void CDXFramework::CreateDepthStencil(const int aWidth, const int aHeight, ID3D11DepthStencilView*& aDepthStencilView, ID3D11ShaderResourceView*& aDepthStencilResource)
{	
	if (aDepthStencilView != nullptr)
		aDepthStencilView->Release();



	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;


	HRESULT result;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = aWidth;
	depthBufferDesc.Height = aHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = GetDepthResourceFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	depthBufferDesc.SampleDesc.Count = 1;				//MSAA magic set too 4 for better stuffs
	depthBufferDesc.SampleDesc.Quality = 0;				//MSAA magic set to 1 to enable magic
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	result = myDevice->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	CHECK_RESULT(result, "Failed to create Texture2D.");
	
	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	result = myDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	CHECK_RESULT(result, "Failed to create Depth Stencil State.");
	myDeviceContext->OMSetDepthStencilState(depthStencilState, 1);
	// Initailze the depth stencil view.



	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // Set to D3D11_DSV_DIMENSION_TEXTURE2DMS for some sort of multiSampling whaop
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// Create the depth stencil view.
	result = myDevice->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &aDepthStencilView);
	CHECK_RESULT(result, "Failed to create Depth Stencil View.");
	//myDeviceContext->OMSetRenderTargets(1, &myRenderTargetView, aDepthStencilView);




	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = GetDepthSRVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;

	result = myDevice->CreateShaderResourceView(depthStencilBuffer, &srvd, &aDepthStencilResource);

	depthStencilState->Release();
	depthStencilBuffer->Release();

}


DXGI_FORMAT CDXFramework::GetDepthResourceFormat(DXGI_FORMAT depthformat)
{
	DXGI_FORMAT resformat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	switch (depthformat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		resformat = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		resformat = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		resformat = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		resformat = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS;
		break;
	}

	return resformat;
}

DXGI_FORMAT CDXFramework::GetDepthSRVFormat(DXGI_FORMAT depthformat)
{
	DXGI_FORMAT srvformat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	switch (depthformat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		srvformat = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		srvformat = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	}
	return srvformat;
}
