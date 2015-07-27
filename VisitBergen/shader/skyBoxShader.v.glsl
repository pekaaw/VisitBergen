#version 330

in vec3 vertex;

uniform mat4 gWVP;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 TexCoord0;

void main()
{
	vec4 spacePosition = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex, 1.0f);
	
	//vec4 WVP_pos = gWVP * vec4(vertex, 1.0f);
	//gl_Position = WVP_pos.xyww;

	gl_Position = spacePosition.xyww;

	TexCoord0 = vertex;
}