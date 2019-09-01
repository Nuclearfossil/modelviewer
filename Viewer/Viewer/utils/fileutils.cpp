#include <io.h>

// Simple utility for checking to see if a file exists
bool FileExists(const char* filename)
{
	bool result = false;

	if (_access(filename, 0) == 0)
		result = true;

	return result;
}
