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
uniform mat4 u_CameraViewProjection;
uniform float u_Thickness;
uniform float u_Fade;
out vec4 o_Color;
void main() {
	vec2 uv = gl_FragCoord.xy / u_WindowSize * 2 - 1;
	float aspect = u_WindowSize.x / u_WindowSize.y;
	uv.x *= aspect;

	uv = (inverse(u_Transform) * inverse(u_CameraViewProjection) * vec4(uv, 0.0, 1.0)).xy;

	//consts
	float thickness = u_Thickness;
	float fade = max(0.000001, u_Fade);
	thickness += fade; //Fill in the hole in the middle

	float distance = 1.0 - length(uv);

	vec3 col = vec3(smoothstep(0.0, fade, distance));
	col *= vec3(smoothstep(thickness, thickness - fade, distance));

	o_Color.rgb = col * u_Color.rgb;
	o_Color.a = col.r;
}

//vec2 uv = gl_FragCoord.xy / u_WindowSize * 2 - 1;
//float aspect = u_WindowSize.x / u_WindowSize.y;
//uv.x *= aspect;
//
//uv = (inverse(u_Transform) * vec4(uv, 0.0, 1.0)).xy;
//
//float distance = 1.0 - length(uv);
//if (distance > 0.0)
//distance = 1.0;
//o_Color = vec4(distance);
//o_Color.rgb *= u_Color;