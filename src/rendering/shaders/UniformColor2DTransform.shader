#type vertex
#version 330 core
layout(location = 0) in vec3 inPosition;
uniform mat3 uniformTransform;
void main() {
	gl_Position.xyz = uniformTransform * vec3(inPosition.x, inPosition.y, 1);
	gl_Position.z = inPosition.z;
	gl_Position.w = 1.0;
}


#type fragment
#version 330 core
uniform vec3 uniformColor;
out vec3 color;
void main() {
	color = uniformColor;
}