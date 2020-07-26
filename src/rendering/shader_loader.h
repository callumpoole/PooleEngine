#pragma once
#include "Core.h"
#include <string_view>

namespace Poole { namespace Rendering {

	GLuint LoadShaders(
		std::string_view vertex_file_path, 
		std::string_view fragment_file_path);

	GLuint LoadShaderLiterals(
		std::string_view vertexShaderCode, 
		std::string_view fragmentShaderCode, 
		std::string_view vertex_file_path = "", 
		std::string_view fragment_file_path = "");

}}
