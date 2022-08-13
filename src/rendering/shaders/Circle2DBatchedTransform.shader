#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
uniform mat4 u_CameraViewProjection;
out vec4 v_Color;
void main() {
	gl_Position = u_CameraViewProjection * vec4(a_Position, 1.0);
	v_Color = a_Color;
}


#type fragment
#version 450 core
in vec4 v_Color;
out vec4 o_Color;
void main() {
	vec4 texColor = v_Color;

	o_Color = texColor;
}