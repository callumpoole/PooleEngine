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



	struct IVirtualMesh
	{
		void InternalInit()
		{
			Init();
			InitUniforms();
		}
		virtual void InitUniforms() {}
		virtual void SetUniforms(GLuint programID) {}
		virtual void Init() {}
		virtual void Render(GLuint programID) {}
		//#todo: find a way to make this constexpr
		bool UsesUniformColor() const;
		bool Uses2DTransform() const;
	};

	struct MeshUniformVirtualBase
		: virtual public IVirtualMesh
	{
		struct UniformData {
			enum EUniformFunction {
				EglUniform3f
			};
			EUniformFunction m_uniformFunc;
			const GLchar* m_shaderMemberName;
			void* m_value;
		};
		std::vector<UniformData> m_UniformData;
		virtual void SetUniforms(GLuint programID);
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

	struct MeshFeature_HasSolidColor_Base {};
	template<typename TLinearColor>
	struct MeshFeature_SolidColor 
		: public MeshFeature_HasSolidColor_Base
		, virtual public MeshUniformVirtualBase
	{
		virtual void InitUniforms() override
		{ 
			//m_UniformData.emplace_back(UniformData::EglUniform3f, (const GLchar*)"uniformColor", &m_color);

			UniformData data;
			data.m_uniformFunc = UniformData::EglUniform3f;
			data.m_shaderMemberName = "uniformColor";
			data.m_value = &m_color;
			m_UniformData.push_back(std::move(data));
		}

		TLinearColor m_color;
	};

	struct MeshFeature_HasNormals {}; //TODO
	struct MeshFeature_HasUVs {}; //TODO



	struct MeshFeature_HasDynamicPosition_Base {};
	template<typename TPosition>
	struct MeshFeature_HasDynamicPosition : public MeshFeature_HasDynamicPosition_Base
	{
		TPosition m_position;
	};

	struct MeshFeature_HasDynamicRotation_Base {};
	template<typename TRotation>
	struct MeshFeature_HasDynamicRotation : public MeshFeature_HasDynamicRotation_Base
	{
		TRotation m_rotation;
	};

	struct MeshFeature_HasDynamicScale_Base {};
	template<typename TScale>
	struct MeshFeature_HasDynamicScale : public MeshFeature_HasDynamicScale_Base
	{
		TScale m_scale;
	};


	struct MeshFeatureCollection_2DTransform
		: MeshFeature_HasDynamicPosition<fvec2>
		, MeshFeature_HasDynamicRotation<f32>
		, MeshFeature_HasDynamicScale<fvec2>
	{
	};








	struct StaticMeshNoIndiciesSolidColor3
		: virtual public IVirtualMesh
		, public MeshFeature_HasVerts<Vertex>
		, public MeshFeature_SolidColor<fcolor3>
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};
	struct StaticMeshNoIndiciesSolidColor3_2DTransform
		: public StaticMeshNoIndiciesSolidColor3
		, public MeshFeatureCollection_2DTransform
	{
		
	};



	struct StaticMeshSolidColor3
		: virtual public IVirtualMesh
		, public MeshFeature_HasVerts<Vertex>
		, public MeshFeature_HasIndicies
		, public MeshFeature_SolidColor<fcolor3>
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};



	struct StaticMeshVertexColor3
		: virtual public IVirtualMesh
		, public MeshFeature_HasVerts<VertexWithColor3>
		, public MeshFeature_HasIndicies
	{
		virtual void Init() override;
		virtual void Render(GLuint programID) override;
	};
}
