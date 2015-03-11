#version 130

in vec3 vertex;
in vec3 normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

out vec3 N;
out vec3 v;

void main()
{
	// vertex position in camera space (3D)
	v = vec3(ModelMatrix * vec4(vertex, 1));
	//v = vec3(ModelMatrix * ViewMatrix * vec4(vertex, 1));

	// Vertex normal in camera space
	N = normalize(NormalMatrix * normal);

	// Calculate vertex position on screen
	//gl_Position = ModelMatrix * ViewMatrix * ProjectionMatrix * vec4(vertex, 1);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex, 1.0f);
	//gl_Position = ViewMatrix * vec4(vertex, 1.0f);
}