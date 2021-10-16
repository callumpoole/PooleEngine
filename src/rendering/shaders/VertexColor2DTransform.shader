#type vertex
#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
uniform mat3 uniformTransform;
out vec3 theColor;
void main() {
	gl_Position.xyz = uniformTransform * vec3(inPosition.x, inPosition.y, 1);
	gl_Position.z = inPosition.z;
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