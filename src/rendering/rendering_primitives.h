#pragma once

#include "core.h"
#include <vector>
#include <type_traits>

namespace Poole::Rendering
{
	using Vertex = glm::vec3;

#pragma warning(disable : 4201) //DISABLE nameless struct/union
	struct VertexWithColor3
	{
		union {
			struct
			{
				Vertex m_position;
				fColor3 m_color;
			};
			struct { f32 x, y, z,    r, g, b; }; //I feel no need for m_ here
		};
	};
	struct VertexWithColor4
	{
		union {
			struct
			{
				Vertex m_position;
				fColor4 m_color;
			};
			struct { f32 x, y, z,    r, g, b, a; }; //I feel no need for m_ here
		};
	};
#pragma warning(default : 4201) //ENABLE nameless struct/union


	template<typename TVertex>
	struct MeshBase_HasVerts
	{
		std::vector<TVertex> m_verts;
		GLuint m_vertexbuffer; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshBase_HasIndicies
	{
		std::vector<int> m_indices;
		GLuint m_elementbuffer; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshBase_HasSolidColor_Base {};
	template<typename TLinearColor>
	struct MeshBase_HasSolidColor : public MeshBase_HasSolidColor_Base
	{
		TLinearColor m_color;
	};

	struct MeshBase_HasNormals {}; //TODO

	struct MeshBase_HasUVs {}; //TODO



	struct IMesh
	{
		virtual void Init() {}
		virtual void Render(GLuint /*programID*/) {}
		bool UsesUniformColor() const { return dynamic_cast<const MeshBase_HasSolidColor_Base*>(this); }
	};

	struct MeshBasicNoIndiciesSolidColor3
		: public MeshBase_HasVerts<Vertex>
		, public MeshBase_HasSolidColor<fColor3>
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};

	struct MeshBasicSolidColor3
		: public MeshBase_HasVerts<Vertex>
		, public MeshBase_HasIndicies
		, public MeshBase_HasSolidColor<fColor3>
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};

	struct MeshBasicVertexColor3
		: public MeshBase_HasVerts<VertexWithColor3>
		, public MeshBase_HasIndicies
		, public IMesh
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};
}
