#include "mainmenu.h"

#include "imgui.h"
#include "imgui_impl_win32.h"

namespace MainMenu
{
	void Update(AppState& state)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open","CTRL+O",false, !state.Waiting))
				{
					state.OpenFile = true;
					state.Waiting = true;
				}
				if (ImGui::MenuItem("Quit", "CTRL+Q", false, !state.Waiting))
				{
					state.ShouldExit = true;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}