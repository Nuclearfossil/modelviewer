#include <io.h>
#include <windows.h>

// Simple utility for checking to see if a file exists
bool FileExists(const char* filename)
{
	bool result = false;

	if (_access(filename, 0) == 0)
		result = true;

	return result;
}

bool OSOpenFile(char* filename)
{
	bool result = false;
	const int BUFSIZE = 1024;
	WCHAR buffer[BUFSIZE] = { 0 };
	OPENFILENAME ofns = { 0 };
	ofns.lStructSize = sizeof(ofns);
	ofns.lpstrFile = buffer;
	ofns.nMaxFile = BUFSIZE;
	ofns.lpstrTitle = TEXT("Select Input FBX file");
	ofns.lpstrFilter = TEXT("FBX Files (*.fbx)\0*.fbx\0");
	if (GetOpenFileName(&ofns) == TRUE)
	{
		size_t i;
		wcstombs_s(&i, filename, (size_t)1024, ofns.lpstrFile, (size_t)1024);
		result = true;
	}

	return result;
}

bool NormalizePath(char* filename)
{
	bool result = false;

	// test the filename one character at a time
	for (size_t index = 0; filename[index] != 0; index++)
	{
		char value = filename[index];
		if (value == '/')
		{
			filename[index] = '\\';
			result = true;
		}
	}

	return result;
}