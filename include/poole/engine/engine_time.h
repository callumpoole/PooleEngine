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
		u64 m_TickCount;
		u64 m_FrameNS;
		f32 m_FrameMS;
		f32 m_DeltaTime;
		f32 m_FPS;
	
		i64 m_LaunchSinceEpochNS;
		i64 m_FirstTickSinceEpochNS;
		f32 m_SecondsSinceLaunch;
		f32 m_SecondsSinceFirstTick;
	
		f32 m_AccDeltaTimeThisSecond;
		u64 m_AccTicksThisSecond;
		f32 m_AvgDeltaTime;
		f32 m_AvgFPS;
	};
}