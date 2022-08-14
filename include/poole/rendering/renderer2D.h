#pragma once

#define UNBATCHED_RENDERER 0
#if UNBATCHED_RENDERER
	#include "unbatched_renderer2D.h"
#endif

#define BATCHED_RENDERER 1
#if BATCHED_RENDERER
	#include "batched_renderer2D.h"
#endif

namespace Poole::Rendering {

#if UNBATCHED_RENDERER && BATCHED_RENDERER
	#define BATCHED_RENDERER_PRECEDENCE 1
	#if BATCHED_RENDERER_PRECEDENCE
		using Renderer2D = BatchedRenderer2D;
	#else
		using Renderer2D = UnbatchedRenderer2D;
	#endif
#elif BATCHED_RENDERER
	using Renderer2D = BatchedRenderer2D;
#else
	using Renderer2D = UnbatchedRenderer2D;
#endif

}
