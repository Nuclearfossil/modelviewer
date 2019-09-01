#include <Windows.h>
#include <tchar.h>


#include "..\\imgui\imgui.h"
#include "..\\renderer.h"

#include "oswindowutils.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace OsWindowUtils
{
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, OsWindowUtils::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ModelViewer"), NULL };

	HWND CreateMainWindow(nlohmann::json& settings)
	{
		auto windowSettings = settings["window"];

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

	void CleanupMainWindow()
	{
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
	}

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
				ResizeRenderer((UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); // I don't like this. It feels like a command that needs to be broadcast.
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
}