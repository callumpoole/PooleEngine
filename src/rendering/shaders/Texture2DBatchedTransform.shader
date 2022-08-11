#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexChords;
layout(location = 3) in float a_TexIndex;
uniform mat4 u_CameraViewProjection;
out vec4 v_Color;
out vec2 v_TexCoord;
void main() {
	gl_Position = u_CameraViewProjection * vec4(a_Position, 1.0);
	v_TexCoord = a_TexChords;
	v_Color = a_Color;
}


#type fragment
#version 450 core
layout(binding = 0) uniform sampler2D u_Textures[32];
in vec4 v_Color;
in vec2 v_TexCoord;
out vec4 o_Color;
void main() {
	o_Color = texture(u_Textures[0], v_TexCoord) * v_Color;
}