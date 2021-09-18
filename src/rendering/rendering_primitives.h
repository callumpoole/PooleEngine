#pragma once

#include "core.h"
#include <vector>

namespace Poole::Rendering
{
	using Vertex = glm::vec3;

#pragma warning(disable : 4201) //DISABLE nameless struct/union
	struct VertexWithColor3
	{
		union {
			struct { f32 x, y, z,    r, g, b; }; //I feel no need for m_ here
			struct
			{
				Vertex m_position;
				LinearColor3 m_color;
			};
		};
	};
	struct VertexWithColor4
	{
		union {
			struct { f32 x, y, z,    r, g, b, a; }; //I feel no need for m_ here
			struct
			{
				Vertex m_position;
				LinearColor4 m_color;
			};
		};
	};
#pragma warning(default : 4201) //ENABLE nameless struct/union


	template<typename TVertex>
	struct MeshBasicNoIndicies
	{
		std::vector<TVertex> m_verts;
		GLuint m_vertexbuffer; //#todo: Giving each its own buffer might fragment memory
	};
	template<typename TVertex>
	struct MeshBasic : public MeshBasicNoIndicies<TVertex>
	{
		std::vector<int> m_indices;
		GLuint m_elementbuffer; //#todo: Giving each its own buffer might fragment memory
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
		virtual void Render(GLuint /*programID*/) {}
		virtual bool UsesUniformColor() { return false; }
	};

	struct MeshBasicNoIndiciesSolidColor3
		: public MeshBasicSolidColor<MeshBasicNoIndicies<Vertex>, LinearColor3>
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
		virtual bool UsesUniformColor() override { return true; }
	};

	struct MeshBasicSolidColor3
		: public MeshBasicSolidColor<MeshBasic<Vertex>, LinearColor3>
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
		virtual bool UsesUniformColor() override { return true; }
	};

	struct MeshBasicVertexColor3
		: public MeshBasic<VertexWithColor3>
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
		virtual bool UsesUniformColor() override { return false; }
	};
}
