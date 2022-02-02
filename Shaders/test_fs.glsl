#version 460
in vec2 o_TexCoords;
in vec3 o_Normal;
in vec3 o_FragPos;
in vec4 o_FragPosLightSpace;

void main()
{
	gl_FragColor = vec4(0.3f, 0.8f, 0.4f, 1.0f);
}