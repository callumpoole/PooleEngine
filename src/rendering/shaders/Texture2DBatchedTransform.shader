//CREDIT TO: TheCherno / Hazel Engine for this

#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexChords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
uniform mat4 u_CameraViewProjection;
out vec4 v_Color;
out vec2 v_TexChords;
out float v_TexIndex;
out float v_TilingFactor;
void main() {
	gl_Position = u_CameraViewProjection * vec4(a_Position, 1.0);
	v_Color = a_Color;
	v_TexChords = a_TexChords;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
}


#type fragment
#version 450 core
layout(binding = 0) uniform sampler2D u_Textures[32];
in vec4 v_Color;
in vec2 v_TexChords;
in float v_TexIndex;
in float v_TilingFactor;
out vec4 o_Color;
void main() {
	vec4 texColor = v_Color;


	switch (int(v_TexIndex))
	{
	case  0: texColor *= texture(u_Textures[ 0], v_TexChords * v_TilingFactor); break;
	case  1: texColor *= texture(u_Textures[ 1], v_TexChords * v_TilingFactor); break;
	case  2: texColor *= texture(u_Textures[ 2], v_TexChords * v_TilingFactor); break;
	case  3: texColor *= texture(u_Textures[ 3], v_TexChords * v_TilingFactor); break;
	case  4: texColor *= texture(u_Textures[ 4], v_TexChords * v_TilingFactor); break;
	case  5: texColor *= texture(u_Textures[ 5], v_TexChords * v_TilingFactor); break;
	case  6: texColor *= texture(u_Textures[ 6], v_TexChords * v_TilingFactor); break;
	case  7: texColor *= texture(u_Textures[ 7], v_TexChords * v_TilingFactor); break;
	case  8: texColor *= texture(u_Textures[ 8], v_TexChords * v_TilingFactor); break;
	case  9: texColor *= texture(u_Textures[ 9], v_TexChords * v_TilingFactor); break;
	case 10: texColor *= texture(u_Textures[10], v_TexChords * v_TilingFactor); break;
	case 11: texColor *= texture(u_Textures[11], v_TexChords * v_TilingFactor); break;
	case 12: texColor *= texture(u_Textures[12], v_TexChords * v_TilingFactor); break;
	case 13: texColor *= texture(u_Textures[13], v_TexChords * v_TilingFactor); break;
	case 14: texColor *= texture(u_Textures[14], v_TexChords * v_TilingFactor); break;
	case 15: texColor *= texture(u_Textures[15], v_TexChords * v_TilingFactor); break;
	case 16: texColor *= texture(u_Textures[16], v_TexChords * v_TilingFactor); break;
	case 17: texColor *= texture(u_Textures[17], v_TexChords * v_TilingFactor); break;
	case 18: texColor *= texture(u_Textures[18], v_TexChords * v_TilingFactor); break;
	case 19: texColor *= texture(u_Textures[19], v_TexChords * v_TilingFactor); break;
	case 20: texColor *= texture(u_Textures[20], v_TexChords * v_TilingFactor); break;
	case 21: texColor *= texture(u_Textures[21], v_TexChords * v_TilingFactor); break;
	case 22: texColor *= texture(u_Textures[22], v_TexChords * v_TilingFactor); break;
	case 23: texColor *= texture(u_Textures[23], v_TexChords * v_TilingFactor); break;
	case 24: texColor *= texture(u_Textures[24], v_TexChords * v_TilingFactor); break;
	case 25: texColor *= texture(u_Textures[25], v_TexChords * v_TilingFactor); break;
	case 26: texColor *= texture(u_Textures[26], v_TexChords * v_TilingFactor); break;
	case 27: texColor *= texture(u_Textures[27], v_TexChords * v_TilingFactor); break;
	case 28: texColor *= texture(u_Textures[28], v_TexChords * v_TilingFactor); break;
	case 29: texColor *= texture(u_Textures[29], v_TexChords * v_TilingFactor); break;
	case 30: texColor *= texture(u_Textures[30], v_TexChords * v_TilingFactor); break;
	case 31: texColor *= texture(u_Textures[31], v_TexChords * v_TilingFactor); break;
	}
	o_Color = texColor;
}