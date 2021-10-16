#type vertex
#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
out vec3 theColor;
void main() {
	gl_Position.xyz = inPosition;
	gl_Position.w = 1.0;
	theColor = inColor;
}


#type fragment
#version 330 core
in vec3 theColor;
out vec3 color;
void main() {
	color = theColor;
}
