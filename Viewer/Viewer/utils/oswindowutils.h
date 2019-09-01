#pragma once

#include <Windows.h>
#include "..\\utils\json.hpp"

namespace OsWindowUtils
{
	HWND CreateMainWindow(nlohmann::json& gSettings);
	void CleanupMainWindow();
}
