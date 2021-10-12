#pragma once

namespace Poole::Rendering::ShaderLiterals
{
	namespace UniformColor
	{
		const char* vertexShader = R"(
			#version 330 core
			layout(location = 0) in vec3 inPosition;
			void main(){
			  gl_Position.xyz = inPosition;
			  gl_Position.w = 1.0;
			}
		)";
		const char* fragmentShader = R"(
			#version 330 core
			uniform vec3 uniformColor;
			out vec3 color;
			void main(){
			  color = uniformColor;
			}
		)";
	}

	namespace VertexColor
	{
		const char* vertexShader = R"(
			#version 330 core
			layout(location = 0) in vec3 inPosition;
			layout(location = 1) in vec3 inColor;
			out vec3 theColor;
			void main(){
			  gl_Position.xyz = inPosition;
			  gl_Position.w = 1.0;
			  theColor = inColor;
			}
		)";
		const char* fragmentShader = R"(
			#version 330 core
			in vec3 theColor;
			out vec3 color;
			void main(){
			  color = theColor;
			}
		)";
	}

	namespace UniformColor_Transform2D
	{
		const char* vertexShader = R"(
			#version 330 core
			layout(location = 0) in vec3 inPosition;
			uniform vec2 uniformPosition;
			uniform float uniformRotation;
			uniform vec2 uniformScale;
			void main(){
			  gl_Position.xy = inPosition.xy + uniformPosition.xy;
			  gl_Position.z = inPosition.z;
			  gl_Position.w = 1.0;
			}
		)";
		const char* fragmentShader = R"(
			#version 330 core
			uniform vec3 uniformColor;
			out vec3 color;
			void main(){
			  color = uniformColor;
			}
		)";
	}

	namespace VertexColor_Transform2D
	{
		const char* vertexShader = R"(
			#version 330 core
			layout(location = 0) in vec3 inPosition;
			layout(location = 1) in vec3 inColor;
			uniform vec2 uniformPosition;
			uniform float uniformRotation;
			uniform vec2 uniformScale;
			out vec3 theColor;
			void main(){
			  gl_Position.xy = inPosition.xy + uniformPosition.xy;
			  gl_Position.z = inPosition.z;
			  gl_Position.w = 1.0;
			  theColor = inColor;
			}
		)";
		const char* fragmentShader = R"(
			#version 330 core
			in vec3 theColor;
			out vec3 color;
			void main(){
			  color = theColor;
			}
		)";
	}
}