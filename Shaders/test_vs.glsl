#version 460

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform vec3 u_CameraPosition;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec2 o_TexCoords;
out vec3 o_Normal;
out vec3 o_FragPos;
out vec4 o_FragPosLightSpace;

void main()
{
	o_Normal = normalize(a_Normal);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;
}