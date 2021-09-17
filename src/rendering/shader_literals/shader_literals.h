#pragma once

namespace Poole::Rendering::ShaderLiterals {

	//Don't modify
	const char* basicVertexShader = R"(
		#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		void main(){
		  gl_Position.xyz = vertexPosition_modelspace;
		  gl_Position.w = 1.0;
		}
	)";

	//Don't modify
	const char* basicFragmentShader = R"(
		#version 330 core
		out vec3 color;
		void main(){
		  color = vec3(1,0,0);
		}
	)";

	//For playing with
	const char* currentVertexShader = R"(
		#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		void main(){
		  gl_Position.xyz = vertexPosition_modelspace;
		  gl_Position.w = 1.0;
		}
	)";

	//For playing with
	const char* currentFragmentShader = R"(
		#version 330 core
		out vec3 color;
		void main(){
		  color = vec3(1,0,0);
		}
	)";

}