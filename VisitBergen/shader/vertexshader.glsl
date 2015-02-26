#version 330

in vec3 position;

uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * ModelMatrix * vec4(position, 1.0);
}