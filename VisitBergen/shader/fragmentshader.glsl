#version 330

// Sampler to access texture
uniform sampler2D TextureSampler;

// Per vertex texture coordinates
in vec2 tex_coords_to_fragment;

out vec4 out_color;

void main()
{
	out_color = texture2D(TextureSampler, tex_coords_to_fragment.st);
}