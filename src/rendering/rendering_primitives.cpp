#include "rendering_primitives.h"
#include "renderer.h"

namespace Poole::Rendering
{
	void IMeshBase::SetUniforms(GLuint programId)
	{
		SetUniform(programId, "u_cameraViewProjection", Poole::Rendering::Renderer::GetCamera().GetViewProjectionMatrix());
	}
}
