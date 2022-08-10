#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexChords;
layout(location = 3) in float a_TexIndex;
uniform mat4 u_Transform;
uniform mat4 u_CameraViewProjection;
out vec2 texCoord;
void main() {
	gl_Position = u_CameraViewProjection * u_Transform * vec4(a_Position, 1.0);
	texCoord = a_TexChords;
}


#type fragment
#version 330 core
uniform sampler2D tex0;
in vec2 texCoord;
uniform vec4 u_Color;
out vec4 o_Color;
void main() {
	o_Color = /*u_Color **/ texture(tex0, texCoord);
}