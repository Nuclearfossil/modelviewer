#include <fstream>
#include <iostream>
#include "utils\oswindowutils.h"
#include <tchar.h>

#include <thread>

//#include "imgui.h"
//#include "imgui_impl_win32.h"

#include "fbxLoader.h"
#include "renderer.h"

#include "gui/guisubsystem.h"
#include "gui/mainmenu.h"
#include "gui/applog.h"
#include "processors/mainprocessor.h"
#include "utils/json.hpp"
#include "utils/fileutils.h"

#include "Viewer.h"

int main()
{
	AppState appState;
	InitAppState(appState);

	nlohmann::json settings;
	GetSettings(settings);

	HWND hwnd = OsWindowUtils::CreateMainWindow(settings);

	// Initialize the Graphics subsystem
	Renderer* pRenderer = CreateRenderer(hwnd);

	if (pRenderer == nullptr)
	{
		OsWindowUtils::CleanupMainWindow();
		std::cerr << "Unable to create render subsystem!" << std::endl;
		return -1;
	}

	// Show our window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Set up our thread from processing non-graphical stuff
	std::thread mainProcessor(MainProcessor, std::ref(appState));

	// Set up the gui subsystem
	gAppLog.Add("Initializing the GUI Subsystem\n");
	GuiSubsystem::Init(hwnd);

	pRenderer->InitGui();

	// Main loop
	gAppLog.Add("Beginning the Message Pump\n");
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

			// A dummy gui here just to bide us over.
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			if (GetScene() != nullptr && !appState.Waiting)
			{
				ImGui::Text("We have a mesh loaded!");
			}

			ImGui::End();

			bool value = true;;
			ShowExampleAppLog(&value, &gAppLog);
		}

		GuiSubsystem::End();

		pRenderer->Present(true); // present with sync
	}

	if (mainProcessor.joinable())
	{
		mainProcessor.detach();
	}

	::DestroyWindow(hwnd);
	OsWindowUtils::CleanupMainWindow();

	return 0;
}

void InitAppState(AppState& appState)
{
	appState.OpenFile = false;
	appState.ShouldExit = false;
	appState.Waiting = false;
}

void GetSettings(nlohmann::json& settings)
{
	if (FileExists("settings.json"))
	{
		std::ifstream settingsStream("settings.json");
		settingsStream >> settings;
	}
	else
	{
		settings["window"]["posx"] = 100;
		settings["window"]["posy"] = 50;
		settings["window"]["width"] = 1500;
		settings["window"]["height"] = 960;
	}
}
