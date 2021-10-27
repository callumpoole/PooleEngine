#type vertex
#version 330 core
layout(location = 0) in vec3 inPosition;
uniform mat4 u_cameraViewProjection;
void main() {
	gl_Position = u_cameraViewProjection * vec4(inPosition, 1.0);
	gl_Position.w = 1.0;
}


#type fragment
#version 330 core
uniform vec4 u_Color;
out vec4 color;
void main() {
	color = u_Color;
}
