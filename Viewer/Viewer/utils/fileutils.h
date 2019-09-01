#pragma once

// Simple utility for checking to see if a file exists
bool FileExists(const char* filename);
bool OSOpenFile(char* filename);
bool NormalizePath(char* filename);  // In place file path normalization