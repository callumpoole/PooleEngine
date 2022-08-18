#pragma once

#include "core.h"

namespace Poole
{
	struct EngineTime
	{
		friend class Engine;
	public:
		u64 GetTickCount() const { return m_TickCount; }
		f32 GetVolatileFPS() const { return m_FPS; }
		f32 GetAvgFPS() const { return m_AvgFPS; }
	private:
		u64 m_TickCount = 0;
		u64 m_FrameNS = 0;
		f32 m_FrameMS = 0;
		f32 m_DeltaTime = 0;
		f32 m_FPS = 0;
	
		i64 m_LaunchSinceEpochNS = 0;
		i64 m_FirstTickSinceEpochNS = 0;
		f32 m_SecondsSinceLaunch = 0;
		f32 m_SecondsSinceFirstTick = 0;
	
		f32 m_AccDeltaTimeThisSecond = 0;
		u64 m_AccTicksThisSecond = 0;
		f32 m_AvgDeltaTime = 0;
		f32 m_AvgFPS = 0;
	};
}