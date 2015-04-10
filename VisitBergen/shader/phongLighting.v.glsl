#version 150

in vec3 vertex;
in vec2 tex_coords;
in vec3 normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
//uniform mat4 NormalMatrix;

out vec3 N;
out vec3 v;
out vec2 textureCoordinates;

void main()
{
	// transfer texture coordinates to fragmentshader
	textureCoordinates = tex_coords;

	// vertex position in camera space (3D)
	//v = vec3(ViewMatrix * ModelMatrix * vec4(vertex, 1));
	v = vec3(ModelMatrix * vec4(vertex, 1));
	//v = vec3(vec4(vertex, 1));
	//v = vec3(ModelMatrix * ViewMatrix * vec4(vertex, 0));

	// Vertex normal in camera space
	N = normalize(transpose(inverse(ModelMatrix)) * vec4(normal, 0)).xyz;
	//N = normalize(NormalMatrix * vec4(normal, 0)).xyz;
	//N = normalize(normal);

	// Calculate vertex position on screen
	//gl_Position = ModelMatrix * ViewMatrix * ProjectionMatrix * vec4(vertex, 1);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex, 1.0f);
	//gl_Position = ProjectionMatrix * ViewMatrix * vec4(vertex, 1.0f);
	//gl_Position = ViewMatrix * vec4(vertex, 1.0f);
}