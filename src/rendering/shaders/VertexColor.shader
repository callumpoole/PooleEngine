#type vertex
#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
uniform mat4 u_cameraViewProjection;
out vec4 theColor;
void main() {
	gl_Position = u_cameraViewProjection * vec4(inPosition, 1.0);
	gl_Position.w = 1.0;
	theColor = inColor;
}


#type fragment
#version 330 core
in vec4 theColor;
out vec4 color;
void main() {
	color = theColor;
}
