#include <fstream>
#include <iostream>
#include <Windows.h>
#include <tchar.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "fbxLoader.h"
#include "renderer.h"
#include "utils/json.hpp"
#include "utils/fileutils.h"

// Some globals
nlohmann::json gSettings;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int main()
{
	if (FileExists("settings.json"))
	{
		std::ifstream settings("settings.json");
		settings >> gSettings;
	}
	else
	{
		gSettings["window"]["posx"] = 100;
		gSettings["window"]["posy"] = 50;
		gSettings["window"]["width"] = 1500;
		gSettings["window"]["height"] = 960;
	}

	auto windowSettings = gSettings["window"];

	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ModelViewer"), NULL };
	::RegisterClassEx(&wc);
	HWND hwnd = ::CreateWindow(wc.lpszClassName, 
		_T("Model Viewer"), 
		WS_OVERLAPPEDWINDOW, 
		windowSettings["posx"], 
		windowSettings["posy"], 
		windowSettings["width"], 
		windowSettings["height"], 
		NULL, NULL, 
		wc.hInstance, 
		NULL);

	// Show our window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

	}

	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);
	return 0;

	//std::string pFile("D:\\dev\\modelviewer\\Viewer\\assets\\ss\\SportCar20.FBX");

	//auto scene = OpenFile(pFile);

	//if (!scene)
	//{
	//	std::cout << "Unable to open file " << pFile << std::endl;
	//	return -1;
	//}

    // std::cout << "Hello World!\n";

	return 0;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_SIZE:
		//if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		//{
		//	CleanupRenderTarget();
		//	g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
		//	CreateRenderTarget();
		//}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}