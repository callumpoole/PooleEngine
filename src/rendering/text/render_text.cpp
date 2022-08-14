#include "poole/rendering/text/render_text.h"

#include "poole/rendering/text/render_font.h"
#include "poole/rendering/renderer2D.h"

namespace Poole::Rendering
{
	void RenderText::SetSize(float size)
	{
		m_Transform.scale = { size,size };
	}

	void RenderText::SetText(std::string_view text)
	{
		ftransform2D Trans = m_Transform;

		for (const char c : text)
		{
			Renderer2D::DrawSubTexturedQuad(Trans, *m_Font->Convert(c));
			Trans.position.x += Trans.scale.x;
		}
	}
}
