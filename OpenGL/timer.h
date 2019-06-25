#pragma once
#include <windows.h>

class Timer
{
public:
	HANDLE mCurrentThread;
	LARGE_INTEGER mStart;
	LARGE_INTEGER mEnd;
	LARGE_INTEGER mCPUFrequency;
	DWORD_PTR mProcessAffinityMask;
public:
	Timer();
	void Start();
	int GetPassedTicks();
	float GetPassedTime();
};