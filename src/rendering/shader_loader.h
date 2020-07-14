#pragma once
#include "poole.h"
#include <string_view>

namespace Poole { namespace Rendering {

	GLuint loadShaders(
		std::string_view vertex_file_path, 
		std::string_view fragment_file_path);

	GLuint loadShaderLiterals(
		std::string_view vertexShaderCode, 
		std::string_view fragmentShaderCode, 
		std::string_view vertex_file_path = "", 
		std::string_view fragment_file_path = "");

}}
