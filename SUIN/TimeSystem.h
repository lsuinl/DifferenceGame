#pragma once

namespace time
{
	void InitTime();

	void UpdateTime();

	const float GetFrameRate();

	const ULONGLONG GetDeltaTime();

}


namespace high_resolution_time
{
	void InitTime();

	void UpdateTime();

	const float GetFrameRate();

	const float GetDeltaTime();
}