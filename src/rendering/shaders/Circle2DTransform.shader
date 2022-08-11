#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
void main() {
	gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
uniform vec4 u_Color;
uniform vec2 u_WindowSize;
uniform mat4 u_InvTransform_InvCameraViewProj;
uniform float u_Thickness;
uniform float u_Fade; //NOTE: Should have a max(input, 0.000001f) in C++, moved there for performance
out vec4 o_Color;
void main() {
	vec2 uv = gl_FragCoord.xy / u_WindowSize * 2 - 1;

	uv = (u_InvTransform_InvCameraViewProj * vec4(uv, 0.0, 1.0)).xy;

	//consts
	float thickness = u_Thickness;
	thickness += u_Fade; //This will prevent a hole in the middle when fade approached 0.

	float distance = 1.0 - length(uv);

	vec3 col = vec3(smoothstep(0.0, u_Fade, distance));
	col *= vec3(smoothstep(thickness, thickness - u_Fade, distance));

	o_Color.rgb = col * u_Color.rgb;
	o_Color.a = col.r;
}
