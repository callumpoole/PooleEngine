#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class Image;
	class SubImage;
	class Texture; //Warning, this is a forward decl for something that isn't publicly accessible

	class BatchedRenderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		static void StartBatch();
		static void FlushBatch();
		static void NextBatch();
	};
}