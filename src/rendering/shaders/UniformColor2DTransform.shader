#type vertex
#version 330 core
layout(location = 0) in vec3 inPosition;
uniform mat3 u_Transform;
uniform mat4 u_cameraViewProjection;
void main() {
	gl_Position.xyz = u_Transform * vec3(inPosition.x, inPosition.y, 1);
	gl_Position = u_cameraViewProjection * vec4(gl_Position.xyz, 1.0);
	gl_Position.z = inPosition.z;
	gl_Position.w = 1.0;
}


#type fragment
#version 330 core
uniform vec4 u_Color;
out vec4 color;
void main() {
	color = u_Color;
}