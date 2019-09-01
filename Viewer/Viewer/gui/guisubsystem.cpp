#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"

#include "guisubsystem.h"

namespace GuiSubsystem
{
	void Init(HWND hwnd)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(hwnd);
	}

	void Teardown()
	{}

	void Begin()
	{
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void End()
	{
		ImGui::Render();
	}
}