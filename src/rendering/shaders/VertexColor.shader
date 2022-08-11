#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
uniform mat4 u_CameraViewProjection;
out vec4 io_Color;
void main() {
	gl_Position = u_CameraViewProjection * vec4(a_Position, 1.0);
	gl_Position.w = 1.0;
	io_Color = a_Color;
}


#type fragment
#version 450 core
in vec4 io_Color;
out vec4 o_Color;
void main() {
	o_Color = io_Color;
}
