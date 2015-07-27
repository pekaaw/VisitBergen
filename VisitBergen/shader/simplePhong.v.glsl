#version 150

in vec3 vertex;
in vec2 tex_coords;
in vec3 normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 N;
out vec3 v;
out vec2 textureCoordinates;

void main()
{
	textureCoordinates = tex_coords;

	v = vec3(ModelMatrix * vec4(vertex, 1));

	// Vertex normal in camera space
	N = normalize(transpose(inverse(ModelMatrix)) * vec4(normal, 0)).xyz;


	// Calculate vertex position on screen
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex, 1.0f);
}