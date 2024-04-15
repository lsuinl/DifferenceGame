#include "SUIN.h"
#include "TimeSystem.h"
#include <cmath>

namespace time
{
	ULONGLONG previousTime;
	ULONGLONG currentTime;
	ULONGLONG deltaTime;

	void InitTime()
	{
		previousTime = currentTime = GetTickCount64();
	}

	void UpdateTime()
	{
		previousTime = currentTime;

		currentTime = GetTickCount64();

		deltaTime = currentTime - previousTime;
	}

	const float GetFrameRate()
	{
		if (deltaTime == 0) return 0;

		return ceil(((1000.0f / deltaTime) * 1000) / 1000);
	}

	const ULONGLONG GetDeltaTime() { return deltaTime; }

}

namespace high_resolution_time
{
	LARGE_INTEGER previousTime = { 0 };
	LARGE_INTEGER currentTime = { 0 };
	LARGE_INTEGER frequency = { 0 };

	float deltaTime = 0;

	void InitTime()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&previousTime);
	}

	void UpdateTime()
	{
		previousTime = currentTime;
		QueryPerformanceCounter(&currentTime);

		deltaTime = (currentTime.QuadPart - previousTime.QuadPart) / (frequency.QuadPart / 1000); //ms
	}

	const float GetFrameRate()
	{
		if (deltaTime == 0) return 0;

		return ceil(((1000.0f / deltaTime) * 1000) / 1000);
	}

	const float GetDeltaTime() { return deltaTime; }
}