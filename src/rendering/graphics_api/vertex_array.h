
#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static VertexArray* Create();
	};
}
