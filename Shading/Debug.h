#pragma once
#include <stdio.h> 

// TODO : write log message to output.txt file.
// TODO : debug parameters should support string.format

class Debug
{
public:
	static void Log(const char* message);
	static void LogWarning(const char*message);
	static void LogError(const char*message);
};

