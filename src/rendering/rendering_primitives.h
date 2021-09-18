#pragma once

#include "core.h"
#include <vector>

namespace Poole::Rendering
{
	using Vertex = glm::vec3;

	template<typename TLinearColor>
	struct VertexWithColor
	{
		Vertex m_position;
		TLinearColor m_color;
	};
	using VertexWithColor3 = VertexWithColor<LinearColor3>;
	using VertexWithColor4 = VertexWithColor<LinearColor4>;


	template<typename TVertex>
	struct MeshBasicNoIndicies
	{
		std::vector<TVertex> m_verts;
		GLuint m_vertexbuffer; //#todo: Giving each its own buffer might fragment memory
	};
	template<typename TVertex>
	struct MeshBasic : public MeshBasicNoIndicies<TVertex>
	{
		std::vector<int> m_indicies;
	};

	template<typename TMesh, typename TLinearColor>
	struct MeshBasicSolidColor
	{
		TMesh m_mesh;
		TLinearColor m_color;
	};

	struct IMesh
	{
		virtual void Init() {}
		virtual void Render() {}
	};


	struct MeshBasicNoIndiciesSolidColor3
		: public MeshBasicSolidColor<MeshBasicNoIndicies<Vertex>, LinearColor3>
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render() override;
	};

	struct MeshBasicSolidColor3
		: public MeshBasicSolidColor<MeshBasic<Vertex>, LinearColor3>
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render() override;
	};



	using MeshBasicNoIndiciesVertexColor3	= MeshBasicNoIndicies<VertexWithColor3>;
	using MeshBasicVertexColor3				= MeshBasic<VertexWithColor3>;
}
// END MOVE