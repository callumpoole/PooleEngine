#pragma once

#include "core.h"
#include <vector>
#include <type_traits>

namespace Poole::Rendering
{
	using Vertex = fvec3;

#pragma warning(disable : 4201) //DISABLE nameless struct/union
	template<typename _TVertexPart, typename _TColorPart>
	struct VertexWithColor { };

	template<>
	struct VertexWithColor<Vertex, fcolor3>
	{
		using TVertexPart = Vertex;
		using TColorPart = fcolor3;
		union {
			struct
			{
				TVertexPart m_position;
				TColorPart m_color;
			};
			struct { f32 x, y, z, r, g, b; }; //I feel no need for m_ here
		};
	};
	using VertexWithColor3 = VertexWithColor<Vertex, fcolor3>;

	template<>
	struct VertexWithColor<Vertex, fcolor4>
	{
		using TVertexPart = Vertex;
		using TColorPart = fcolor4;
		union {
			struct
			{
				TVertexPart m_position;
				TColorPart m_color;
			};
			struct { f32 x, y, z, r, g, b, a; }; //I feel no need for m_ here
		};
	};
	using VertexWithColor4 = VertexWithColor<Vertex, fcolor4>;
#pragma warning(default : 4201) //DEFAULT nameless struct/union


	struct IMeshBase
	{
		virtual void Init() {}
		virtual void Render(GLuint /*programId*/) {}
		//#todo: find a way to make this constexpr / remove it
		virtual bool UsesUniformColor3() const { return false; }
		virtual bool Uses2DTransform() const { return false; }
		virtual void SetUniforms(GLuint /*programId*/) {}
	};

	template<typename ... TDecorators>
	struct IMesh 
		: public IMeshBase
		, public TDecorators...
	{
		static_assert((std::is_base_of<IMeshDecoratorBase, TDecorators>::value && ...),
			"All TDecorators must inherit from IMeshUniformDataBase struct.");

		virtual bool UsesUniformColor3() const override
		{
			return (std::is_base_of<MeshUniform_SolidColor<fcolor3>, TDecorators>::value || ...);
		}
		virtual bool Uses2DTransform() const override
		{
			//return (std::is_base_of<MeshUniform_DynamicPosition<fvec2>, TDecorators>::value || ...)
			//	&& (std::is_base_of<MeshUniform_DynamicRotation<f32>, TDecorators>::value || ...)
			//	&& (std::is_base_of<MeshUniform_DynamicScale<fvec2>, TDecorators>::value || ...);
			return (std::is_base_of<MeshUniform_DynamicTransform<fmat3>, TDecorators>::value || ...);
		}
		virtual void SetUniforms(GLuint programId) override
		{
			IMeshBase::SetUniforms(programId); //Super
			(TDecorators::SetInternalUniforms(programId), ...);
		}
	};



	//template <typename T>
	//concept IMesh2 = requires(T& t, const T& tconst) {
	//	t.InternalInit();
	//	t.Init();
	//	t.Render((GLuint)0);
	//	{ tconst.UsesUniformColor3() } -> std::same_as<bool>;
	//	{ tconst.Uses2DTransform() } -> std::same_as<bool>;
	//	t.SetUniforms((GLuint)0);
	//};




	struct IMeshDecoratorBase
	{
		virtual void SetInternalUniforms(const GLuint /*programId*/) { }
	protected:
		template<typename TFloats> void SetUniform(const GLint /*uniform*/, const TFloats /*f*/) { assert(0); }
		template<> void SetUniform(const GLint uniform, const f32 f)   { glUniform1f(uniform, f); }
		template<> void SetUniform(const GLint uniform, const fvec2 v) { glUniform2f(uniform, v.x, v.y); }
		template<> void SetUniform(const GLint uniform, const fvec3 v) { glUniform3f(uniform, v.x, v.y, v.z); }
		template<> void SetUniform(const GLint uniform, const fvec4 v) { glUniform4f(uniform, v.x, v.y, v.z, v.w); }
		template<> void SetUniform(const GLint uniform, const fmat2 v) { glUniformMatrix2fv(uniform, 1, GL_TRUE, (const GLfloat*)&v); }
		template<> void SetUniform(const GLint uniform, const fmat3 v) { glUniformMatrix3fv(uniform, 1, GL_TRUE, (const GLfloat*)&v); }
		template<> void SetUniform(const GLint uniform, const fmat4 v) { glUniformMatrix4fv(uniform, 1, GL_TRUE, (const GLfloat*)&v); }
	};

#define MakeUniform(Name, UniformName, MemberName)													\
	static constexpr const char* MeshUniform_##Name##_Uniform = UniformName;						\
	struct MeshUniform_##Name##Base {};																\
	template<typename T>																			\
	struct MeshUniform_##Name																		\
		: public MeshUniform_##Name##Base															\
		, public IMeshDecoratorBase																	\
	{																								\
		virtual void SetInternalUniforms(const GLuint programId) override							\
		{																							\
			IMeshDecoratorBase::SetInternalUniforms(programId); /*Super*/							\
			SetUniform(glGetUniformLocation(programId, MeshUniform_##Name##_Uniform), MemberName);	\
		}																							\
		T MemberName{};																				\
	};



	MakeUniform(SolidColor, "uniformColor", m_color);
	//MakeUniform(DynamicPosition, "uniformPosition", m_position);
	//MakeUniform(DynamicRotation, "uniformRotation", m_rotation);
	//MakeUniform(DynamicScale, "uniformScale", m_scale); //Issue is default value is 0s not 1s
	//MakeUniform(DynamicShear, "uniformShear", m_shear);
	MakeUniform(DynamicTransform, "uniformTransform", m_transform);

	template<>
	struct MeshUniform_DynamicTransform<fmat3>
		: public MeshUniform_DynamicTransformBase
		, public IMeshDecoratorBase
	{
		virtual void SetInternalUniforms(const GLuint programId) override		
		{																		
			IMeshDecoratorBase::SetInternalUniforms(programId); /*Super*/

			const fmat3 Translation = {
				1, 0, m_position.x,
				0, 1, m_position.y,
				0, 0, 1
			};
			const fmat3 Rotation = {
				cosf(m_rotation), -sinf(m_rotation), 0,
				sinf(m_rotation),  cosf(m_rotation), 0,
							   0,				  0, 1,
			};
			const fmat3 Scale = {
				m_scale.x,		   0, 0,
						0, m_scale.y, 0,
						0,		   0, 1
			};
			const fmat3 Shear = {
				1,		   m_shear.x, 0,
				m_shear.y,		   1, 0,
				0,				   0, 1
			};

			m_transform = Translation * Rotation * Shear * Scale;

			IMeshDecoratorBase::SetUniform(glGetUniformLocation(programId, MeshUniform_DynamicTransform_Uniform), m_transform);
		}																		
		fmat3 m_transform{};
		fvec2 m_position{ 0.f, 0.f };
		f32 m_rotation = 0;
		fvec2 m_scale{1.f, 1.f};
		fvec2 m_shear{0.f, 0.f};
	};






	struct MeshFeature_HasVertsBase {};
	template<typename _TVertex>
	struct MeshFeature_HasVerts : public MeshFeature_HasVertsBase, public IMeshDecoratorBase
	{
		using TVertex = _TVertex;
		std::vector<TVertex> m_verts;
		GLuint m_vertexbuffer = 0; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasIndicies : public IMeshDecoratorBase
	{
		std::vector<int> m_indices;
		GLuint m_elementbuffer = 0; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasNormals : public IMeshDecoratorBase {}; //TODO
	struct MeshFeature_HasUVs : public IMeshDecoratorBase {}; //TODO











	template<class ... TDecorators>
	struct StaticMeshNoIndiciesSolidColor3
		: public IMesh<MeshFeature_HasVerts<Vertex>, 
					   MeshUniform_SolidColor<fcolor3>, 
					   TDecorators...>
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};

	struct StaticMeshNoIndiciesSolidColor3_2DTransform
		: public StaticMeshNoIndiciesSolidColor3<MeshUniform_DynamicTransform<fmat3>>
	{
	};


	template<class ... TDecorators>
	struct StaticMeshSolidColor3
		: public IMesh<MeshFeature_HasVerts<Vertex>,
					   MeshFeature_HasIndicies,
					   MeshUniform_SolidColor<fcolor3>, 
					   TDecorators...>
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};

	struct StaticMeshSolidColor3_2DTransform
		: public StaticMeshSolidColor3<MeshUniform_DynamicTransform<fmat3>>
	{
	};


	template<class ... TDecorators>
	struct StaticMeshVertexColor3
		: public IMesh<MeshFeature_HasVerts<VertexWithColor3>,
					   MeshFeature_HasIndicies,
					   TDecorators...>
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};

	struct StaticMeshVertexColor3_2DTransform
		: public StaticMeshVertexColor3<MeshUniform_DynamicTransform<fmat3>>
	{
	};






	template<class ... TDecorators>
	void StaticMeshNoIndiciesSolidColor3<TDecorators...>::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);
	}
	template<class ... TDecorators>
	void StaticMeshNoIndiciesSolidColor3<TDecorators...>::Render(GLuint programId)
	{
		//Use Shader
		glUseProgram(programId);

		SetUniforms(programId);
		//GLint Loc = glGetUniformLocation(programId, "uniformColor");
		//glUniform3f(Loc, m_color.r, m_color.g, m_color.b);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex) / sizeof(f32),  //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			3 * sizeof(f32),				//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_verts.size()); //Starting from vertex 0; N vertices total.
		glDisableVertexAttribArray(0);
	}


	//---

	template<class ... TDecorators>
	void StaticMeshSolidColor3<TDecorators...>::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);

		//Generate 1 buffer, put the resulting identifier in m_elementbuffer
		glGenBuffers(1, &m_elementbuffer);
		//The following commands will talk about our 'm_elementbuffer' buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		//Give our indices to OpenGL.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}
	template<class ... TDecorators>
	void StaticMeshSolidColor3<TDecorators...>::Render(GLuint programId)
	{
		//Use Shader
		glUseProgram(programId);

		SetUniforms(programId);
		//GLint Loc = glGetUniformLocation(programId, "uniformColor");
		//glUniform3f(Loc, m_color.r, m_color.g, m_color.b);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex) / sizeof(f32),  //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			0,								//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
	}

	//--

	template<class ... TDecorators>
	void StaticMeshVertexColor3<TDecorators...>::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);

		//Generate 1 buffer, put the resulting identifier in m_elementbuffer
		glGenBuffers(1, &m_elementbuffer);
		//The following commands will talk about our 'm_elementbuffer' buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}
	template<class ... TDecorators>
	void StaticMeshVertexColor3<TDecorators...>::Render(GLuint programId)
	{
		//Use Shader
		glUseProgram(programId);

		SetUniforms(programId);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		//0th attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,											//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex::TVertexPart) / sizeof(f32), //size
			GL_FLOAT,									//type
			GL_FALSE,									//normalized?
			sizeof(TVertex),							//stride
			(void*)0									//array buffer offset
		);

		//1st attribute buffer : color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,											//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex::TColorPart) / sizeof(f32),	//size
			GL_FLOAT,									//type
			GL_FALSE,									//normalized?
			sizeof(TVertex),							//stride
			(char*)(sizeof(TVertex::TVertexPart))		//array buffer offset
		);

		//Draw the triangle !
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

}