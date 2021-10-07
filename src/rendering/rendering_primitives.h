#pragma once

#include "core.h"
#include <vector>
#include <type_traits>

namespace Poole::Rendering
{
	using Vertex = fvec3;

#pragma warning(disable : 4201) //DISABLE nameless struct/union
	struct VertexWithColor3
	{
		union {
			struct
			{
				Vertex m_position;
				fcolor3 m_color;
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
				fcolor4 m_color;
			};
			struct { f32 x, y, z,    r, g, b, a; }; //I feel no need for m_ here
		};
	};
#pragma warning(default : 4201) //ENABLE nameless struct/union

	struct IMesh
	{
		void InternalInit() //Could remove if nothing else is needed, Super::Would probably work
		{
			Init();
		}
		virtual void Init() {}
		virtual void Render(GLuint programID) {}
		//#todo: find a way to make this constexpr
		bool UsesUniformColor() const;
		bool Uses2DTransform() const;
		void SetUniforms(GLuint programID);
	};




	struct IMeshUniformDataBase
	{
		virtual void SetInternalUniforms(GLuint programID) { }
	};

	struct MeshUniform_SolidColorBase {};
	template<typename TLinearColor>
	struct MeshUniform_SolidColor : public MeshUniform_SolidColorBase, public IMeshUniformDataBase
	{
		virtual void SetInternalUniforms(GLuint programID) override
		{
			const GLint uniformLocation = glGetUniformLocation(programID, "uniformColor");
			glUniform3f(uniformLocation, m_color[0], m_color[1], m_color[2]);
		}
		TLinearColor m_color;
	};



	struct MeshUniform_DynamicPositionBase {};
	template<typename TPosition>
	struct MeshUniform_DynamicPosition : public MeshUniform_DynamicPositionBase, public IMeshUniformDataBase
	{
		virtual void SetInternalUniforms(GLuint programID) override
		{
			const GLint uniformLocation = glGetUniformLocation(programID, "uniformPosition");
			glUniform3f(uniformLocation, m_color[0], m_color[1], m_color[2]);
		}
		TPosition m_position;
	};

	struct MeshUniform_DynamicRotationBase {};
	template<typename TRotation>
	struct MeshUniform_DynamicRotation : public MeshUniform_DynamicRotationBase, public IMeshUniformDataBase
	{
		virtual void SetInternalUniforms(GLuint programID) override
		{
			const GLint uniformLocation = glGetUniformLocation(programID, "uniformRotation");
			glUniform1f(uniformLocation, m_color[0], m_color[1], m_color[2]);
		}
		TRotation m_rotation;
	};

	struct MeshFeature_HasDynamicScale_Base {};
	template<typename TScale>
	struct MeshUniform_DynamicScale : public MeshFeature_HasDynamicScale_Base, public IMeshUniformDataBase
	{
		virtual void SetInternalUniforms(GLuint programID) override
		{
			const GLint uniformLocation = glGetUniformLocation(programID, "uniformScale");
			glUniform3f(uniformLocation, m_color[0], m_color[1], m_color[2]);
		}
		TScale m_scale;
	};


	#define MeshUniformCollection_2DTransform \
		  MeshUniform_DynamicPosition<fvec2> \
		, MeshUniform_DynamicRotation<f32> \
		, MeshUniform_DynamicScale<fvec2>







	struct IMeshUniformCollectorBase
	{
		virtual void SetAllUniforms(GLuint programID) {}
	};

	template<class ... TDecorators>
	struct IMeshUniformCollector 
		: public IMeshUniformCollectorBase
		, public TDecorators...
	{
		static_assert((std::is_base_of<IMeshUniformDataBase, TDecorators>::value && ...), "All TDecorators must inherit from IMeshUniformDataBase struct.");

		virtual void SetAllUniforms(GLuint programID) override
		{
			(TDecorators::SetInternalUniforms(programID), ...);
		}
	};










	template<typename TVertex>
	struct MeshFeature_HasVerts
	{
		std::vector<TVertex> m_verts;
		GLuint m_vertexbuffer; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasIndicies
	{
		std::vector<int> m_indices;
		GLuint m_elementbuffer; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasNormals {}; //TODO
	struct MeshFeature_HasUVs {}; //TODO












	struct StaticMeshNoIndiciesSolidColor3
		: public IMesh
		, public MeshFeature_HasVerts<Vertex>
		, public IMeshUniformCollector< MeshUniform_SolidColor<fcolor3> >
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};
	struct StaticMeshNoIndiciesSolidColor3_2DTransform
		: public StaticMeshNoIndiciesSolidColor3
		//, public MeshUniformCollection_2DTransform
	{
		
	};



	struct StaticMeshSolidColor3
		: public IMesh
		, public MeshFeature_HasVerts<Vertex>
		, public MeshFeature_HasIndicies
		, public IMeshUniformCollector< MeshUniform_SolidColor<fcolor3> >
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};



	struct StaticMeshVertexColor3
		: public IMesh
		, public MeshFeature_HasVerts<VertexWithColor3>
		, public MeshFeature_HasIndicies
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};
}
