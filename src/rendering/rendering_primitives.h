#pragma once

#include "core.h"
#include <vector>
#include <type_traits>
#include "glm/gtc/type_ptr.hpp"
#include "camera/orthographic_camera.h"

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




	namespace
	{
		template<typename TFloats> void SetUniform(const GLint /*uniform*/, const TFloats /*f*/) { assert(0); }
		template<> void SetUniform(const GLint uniform, const f32 f) { glUniform1f(uniform, f); }
		template<> void SetUniform(const GLint uniform, const fvec2 v) { glUniform2f(uniform, v.x, v.y); }
		template<> void SetUniform(const GLint uniform, const fvec3 v) { glUniform3f(uniform, v.x, v.y, v.z); }
		template<> void SetUniform(const GLint uniform, const fvec4 v) { glUniform4f(uniform, v.x, v.y, v.z, v.w); }
		template<> void SetUniform(const GLint uniform, const fmat2 v) { glUniformMatrix2fv(uniform, 1, GL_TRUE, glm::value_ptr(v)); }
		template<> void SetUniform(const GLint uniform, const fmat3 v) { glUniformMatrix3fv(uniform, 1, GL_TRUE, glm::value_ptr(v)); }
		template<> void SetUniform(const GLint uniform, const fmat4 v) { glUniformMatrix4fv(uniform, 1, GL_TRUE, glm::value_ptr(v)); }

		template<typename TFloats>
		void SetUniform(const GLuint programId, const char* uniformName, const TFloats& v)
		{
			SetUniform(glGetUniformLocation(programId, uniformName), v);
		}
	}




	struct IMeshBase
	{
		virtual void Init() {}
		virtual void Render(GLuint /*programId*/, const OrthographicCamera* /*camera*/) {}
		//#todo: find a way to make this constexpr / remove it
		virtual bool UsesUniformColor3() const { return false; }
		virtual bool Uses2DTransform() const { return false; }
		virtual void SetUniforms(GLuint programId, const OrthographicCamera* camera) 
		{
			SetUniform(programId, "u_cameraViewProjection", camera->GetViewProjectionMatrix());
		}
	};

	template<typename ... TDecorators>
	struct IMesh 
		: public IMeshBase
		, public TDecorators...
	{
		static_assert((std::is_base_of<IMeshDecoratorBase, TDecorators>::value && ...),
			"All TDecorators must inherit from IMeshUniformDataBase struct.");

		template<typename T>
		constexpr bool ContainsDecorator() const
		{
			return (std::is_base_of<T, TDecorators>::value || ...);
		}

		virtual bool UsesUniformColor3() const override
		{
			return ContainsDecorator<MeshUniform_SolidColor<fcolor3>>();
		}
		virtual bool Uses2DTransform() const override
		{
			return ContainsDecorator<MeshUniform_DynamicTransform<fmat3>>();
			//return ContainsDecorator<MeshUniform_DynamicPosition<fvec2>>()
			//	&& ContainsDecorator<MeshUniform_DynamicRotation<f32>>()
			//	&& ContainsDecorator<MeshUniform_DynamicScale<fvec2>>()
			//	&& ContainsDecorator<MeshUniform_DynamicShear<fvec2>>();
		}
		virtual void SetUniforms(GLuint programId, const OrthographicCamera* camera) override
		{
			IMeshBase::SetUniforms(programId, camera); //Super
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
	};

	template<typename T>
	struct MeshUniform_SolidColor : public IMeshDecoratorBase
	{
		virtual void SetInternalUniforms(const GLuint programId) override
		{
			IMeshDecoratorBase::SetInternalUniforms(programId); /*Super*/
			SetUniform(programId, "u_Color", m_color);
		}
		T m_color{};
	};


	template<typename T>
	struct MeshUniform_DynamicTransform { }; //Certain Specializations only
	template<>
	struct MeshUniform_DynamicTransform<fmat3> : public IMeshDecoratorBase
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

			SetUniform(programId, "u_Transform", m_transform);
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
		virtual void Render(GLuint programId, const OrthographicCamera* camera) override;
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
		virtual void Render(GLuint programId, const OrthographicCamera* camera) override;
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
		virtual void Render(GLuint programId, const OrthographicCamera* camera) override;
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
	void StaticMeshNoIndiciesSolidColor3<TDecorators...>::Render(GLuint programId, const OrthographicCamera* camera)
	{
		//Use Shader
		glUseProgram(programId);
		SetUniforms(programId, camera);

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
	void StaticMeshSolidColor3<TDecorators...>::Render(GLuint programId, const OrthographicCamera* camera)
	{
		//Use Shader
		glUseProgram(programId);
		SetUniforms(programId, camera);

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
	void StaticMeshVertexColor3<TDecorators...>::Render(GLuint programId, const OrthographicCamera* camera)
	{
		//Use Shader
		glUseProgram(programId);
		SetUniforms(programId, camera);

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