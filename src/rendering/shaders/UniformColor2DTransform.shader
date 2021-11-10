#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
uniform mat4 u_Transform;
uniform mat4 u_CameraViewProjection;
void main() {
	gl_Position = u_CameraViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core
uniform vec4 u_Color;
out vec4 o_Color;
void main() {
	o_Color = u_Color;
}