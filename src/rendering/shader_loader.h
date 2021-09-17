#pragma once
#include "core.h"
#include <string_view>

namespace Poole::Rendering {

	GLuint LoadShaders(
		std::string_view vertexFilePath, 
		std::string_view fragmentFilePath
	);

	GLuint LoadShaderLiterals(
		std::string_view vertexShaderCode, 
		std::string_view fragmentShaderCode, 
		std::string_view vertexFilePath = "", 
		std::string_view fragmentFilePath = ""
	);

}
