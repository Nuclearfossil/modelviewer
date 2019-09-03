#include "..\\imgui\imgui.h"

// Usage:
//  static AppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct LogData
{
	ImGuiTextBuffer		Buffer;
	ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines

	LogData();

	void Clear();
	void Add(const char* fmt, ...) IM_FMTARGS(2);
};

struct LogViewer
{

	LogData*			Data;
	ImGuiTextFilter     Filter;
	bool                AutoScroll;     // Keep scrolling if already at the bottom

	LogViewer(LogData* data);

	void Clear();

	void Draw(const char* title, bool* p_open = NULL);
};

// Demonstrate creating a simple log window with basic filtering.
void ShowExampleAppLog(bool* p_open, LogData* logData);

extern LogData gAppLog;

