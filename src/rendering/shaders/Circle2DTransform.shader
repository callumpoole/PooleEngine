#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
void main() {
	gl_Position = vec4(a_Position, 1.0);
}


#type fragment
#version 330 core
uniform vec4 u_Color;
uniform vec2 u_WindowSize;
uniform mat4 u_Transform;
out vec4 o_Color;
void main() {
	vec2 uv = gl_FragCoord.xy / u_WindowSize * 2 - 1;
	float aspect = u_WindowSize.x / u_WindowSize.y;
	uv.x *= aspect;

	uv = (inverse(u_Transform) * vec4(uv, 0.0, 1.0)).xy;

	float distance = 1.0 - length(uv);
	if (distance > 0.0)
		distance = 1.0;
	o_Color = vec4(distance);
	o_Color.rgb *= u_Color;
}