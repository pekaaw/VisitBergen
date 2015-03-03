#version 330

uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

in vec3 position;

in vec2 tex_coords;

out vec2 tex_coords_to_fragment;

void main()
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * ModelMatrix * vec4(position, 1.0);

	tex_coords_to_fragment = tex_coords;
}