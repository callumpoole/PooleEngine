#type vertex
#version 330 core
layout(location = 0) in vec3 inPosition;
void main() {
	gl_Position.xyz = inPosition;
	gl_Position.w = 1.0;
}


#type fragment
#version 330 core
uniform vec3 uniformColor;
out vec3 color;
void main() {
	color = uniformColor;
}
