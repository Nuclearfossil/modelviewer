
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include "imgui.h"
// #include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "renderer.h"

// internal functions
bool CreateDeviceD3D(HWND hwnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

// File local data
static ID3D11Device* gD3DDevice = nullptr;
static ID3D11DeviceContext* gD3DDeviceContext = nullptr;
static IDXGISwapChain* gSwapChain = nullptr;
static ID3D11RenderTargetView* gMainRenderTargetView = nullptr;

static Renderer* gRenderer;


Renderer* CreateRenderer(HWND hWnd)
{
	if (!CreateDeviceD3D(hWnd))
	{
		CleanupDeviceD3D();
		return nullptr;
	}

	gRenderer = new Renderer();
	return gRenderer;
}

void CleanupRender()
{
	CleanupDeviceD3D();
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC swapchainDescriptor;
	ZeroMemory(&swapchainDescriptor, sizeof(swapchainDescriptor));
	swapchainDescriptor.BufferCount = 2;
	swapchainDescriptor.BufferDesc.Width = 0;
	swapchainDescriptor.BufferDesc.Height = 0;
	swapchainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDescriptor.BufferDesc.RefreshRate.Numerator = 60;
	swapchainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
	swapchainDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapchainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDescriptor.OutputWindow = hWnd;
	swapchainDescriptor.SampleDesc.Count = 1;
	swapchainDescriptor.SampleDesc.Quality = 0;
	swapchainDescriptor.Windowed = TRUE;
	swapchainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevelArray,
		2,
		D3D11_SDK_VERSION,
		&swapchainDescriptor,
		&gSwapChain,
		&gD3DDevice,
		&featureLevel,
		&gD3DDeviceContext) != S_OK)
	{
		return false;
	}
	CreateRenderTarget();

	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (gSwapChain != nullptr)
	{ 
		gSwapChain->Release(); 
		gSwapChain = nullptr; 
	}

	if (gD3DDeviceContext != nullptr)
	{ 
		gD3DDeviceContext->Release(); 
		gD3DDeviceContext = nullptr; 
	}

	if (gD3DDevice != nullptr)
	{ 
		gD3DDevice->Release(); 
		gD3DDevice = nullptr; 
	}
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	gSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer != nullptr)
	{
		gD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &gMainRenderTargetView);
		pBackBuffer->Release();
	}
}

void CleanupRenderTarget()
{
	if (gMainRenderTargetView) 
	{ 
		gMainRenderTargetView->Release(); 
		gMainRenderTargetView = nullptr; 
	}
}

void ResizeRenderer(UINT width, UINT height)
{
	if (gD3DDevice != nullptr)
	{
		CleanupRenderTarget();
		gSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTarget();
	}
}

// Renderer class methods
Renderer::Renderer()
{
	clearColor[0] = 0.45f;
	clearColor[1] = 0.55f;
	clearColor[2] = 0.60f;
	clearColor[3] = 1.0f;
}

void Renderer::InitGui()
{
	ImGui_ImplDX11_Init(gD3DDevice, gD3DDeviceContext);
}

void Renderer::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
}

void Renderer::Present(bool sync)
{
	gD3DDeviceContext->OMSetRenderTargets(1, &gMainRenderTargetView, nullptr);
	gD3DDeviceContext->ClearRenderTargetView(gMainRenderTargetView, (float*)&clearColor);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	gSwapChain->Present(sync ? 1 : 0, 0);
}