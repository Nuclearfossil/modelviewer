#include <fstream>
#include <iostream>
#include <Windows.h>
#include <tchar.h>

#include <thread>

#include "imgui.h"
#include "imgui_impl_win32.h"

#include "fbxLoader.h"
#include "renderer.h"

#include "gui/guisubsystem.h"
#include "gui/mainmenu.h"
#include "utils/json.hpp"
#include "utils/fileutils.h"

// Some globals
nlohmann::json gSettings;

HWND CreateMainWindow(WNDCLASSEX& wc);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void BuildMenu(volatile AppState* state);

void MainProcessor(AppState& gAppState);

int main()
{
	AppState appState;

	appState.OpenFile = false;
	appState.ShouldExit = false;
	appState.Waiting = false;

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

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ModelViewer"), NULL };

	HWND hwnd = CreateMainWindow(wc);

	// Initialize the Graphics subsystem
	Renderer* pRenderer = CreateRenderer(hwnd);

	if (pRenderer == nullptr)
	{
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		std::cerr << "Unable to create render subsystem!" << std::endl;
		return -1;
	}

	// Show our window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Set up our thread from processing non-graphical stuff
	std::thread mainProcessor(MainProcessor, std::ref(appState));

	// Set up the gui subsystem
	GuiSubsystem::Init(hwnd);

	pRenderer->InitGui();

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && !appState.ShouldExit)
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
		pRenderer->NewFrame();
		GuiSubsystem::Begin();

		{
			MainMenu::Update(appState);
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			if (GetScene() != nullptr)
			{
				ImGui::Text("We have a mesh loaded!");
			}

			ImGui::End();	
		}

		GuiSubsystem::End();

		pRenderer->Present(true); // present with sync
	}

	if (mainProcessor.joinable())
	{
		mainProcessor.detach();
	}

	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

HWND CreateMainWindow(WNDCLASSEX& wc)
{
	auto windowSettings = gSettings["window"];

	// Create application window
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

	return hwnd;
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
		if (wParam != SIZE_MINIMIZED)
			ResizeRenderer((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
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

void MainProcessor(AppState& appState)
{
	while (appState.ShouldExit == false)
	{
		if (appState.OpenFile)
		{
			// open the file
			char filename[1024];
			if (OSOpenFile(filename))
			{
				OpenSceneFile(filename);
				appState.OpenFile = false;
				appState.Waiting = false;
			}
		}
	}

}
