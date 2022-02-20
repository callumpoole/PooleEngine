#pragma once

#include "core.h"

namespace Poole
{
	struct EngineTime
	{
		friend class Engine;
	public:
		u64 GetTickCount() const { return s_TickCount; }
	private:
		u64 s_TickCount;
		u64 s_FrameNS;
		f32 s_FrameMS;
		f32 s_DeltaTime;
		f32 s_FPS;
	
		i64 s_LaunchSinceEpochNS;
		i64 s_FirstTickSinceEpochNS;
		f32 s_SecondsSinceLaunch;
		f32 s_SecondsSinceFirstTick;
	
		f32 s_AccDeltaTimeThisSecond;
		u64 s_AccTicksThisSecond;
		f32 s_AvgDeltaTime;
		f32 s_AvgFPS;
	};
}