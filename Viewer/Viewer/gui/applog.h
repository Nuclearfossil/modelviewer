#include "..\\imgui\imgui.h"

// Usage:
//  static AppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct AppLog
{
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
	bool                AutoScroll;     // Keep scrolling if already at the bottom

	AppLog();

	void Clear();

	void AddLog(const char* fmt, ...) IM_FMTARGS(2);

	void Draw(const char* title, bool* p_open = NULL);
};

// Demonstrate creating a simple log window with basic filtering.
void ShowExampleAppLog(bool* p_open);
