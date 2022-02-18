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
uniform mat4 u_Transform;
uniform vec4 u_Color;
uniform vec2 u_WindowSize;
out vec4 o_Color;
void main() {
	vec2 uv = gl_FragCoord.xy / u_WindowSize * 2 - 1;
	float aspect = u_WindowSize.x / u_WindowSize.y;
	uv.x *= aspect;

	float distance = 1.0 - length(uv);
	if (distance > 0.0)
		distance = 1.0;
	o_Color = vec4(distance);
	o_Color.rgb *= u_Color;
}