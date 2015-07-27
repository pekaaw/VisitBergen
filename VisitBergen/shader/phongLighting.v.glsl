#version 150

in vec3 vertex;
in vec2 tex_coords;
in vec3 normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
//uniform mat4 NormalMatrix;
uniform float WaveTime;

out vec3 N;
out vec3 v;
out vec2 textureCoordinates;
out float isSea;

void main()
{
	// transfer texture coordinates to fragmentshader
	textureCoordinates = tex_coords;

	isSea = 0.0f;

	vec3 vert = vertex;
	if(vert.y <= 0)
	{
		//vert.y = sin(100.6 * vert.x + WaveTime) * cos(100.6 * vert.z + WaveTime) * 100.0;
//		vert.x = vert.x + clamp(vert.x * cos(vert.x), 0.0f, 1.0f);
//		vert.z = vert.z + clamp(vert.z * sin(vert.z), 0.0f, 1.0f);
//		vert.y = sin(vert.x + WaveTime * 8) * cos(vert.z + WaveTime * 8);
//		isSea = 1.0f;
		//vert.y = sin(WaveTime * 8) * 100;
	}

	// vertex position in camera space (3D)
	//v = vec3(ViewMatrix * ModelMatrix * vec4(vertex, 1));
	v = vec3(ModelMatrix * vec4(vert, 1));
	//v = vec3(vec4(vertex, 1));
	//v = vec3(ModelMatrix * ViewMatrix * vec4(vertex, 0));

	if(v.y <= 0)
	{
		v.x = v.x + clamp(v.x * cos(v.x), 0.0f, 1.0f);
		v.z = v.z + clamp(v.z * sin(v.z), 0.0f, 1.0f);
		v.y = sin(v.x + WaveTime * 8) * cos(v.z + WaveTime * 8);
		isSea = 1.0f;
	}

	vec3 norm = normal;

	if( v.y <= 0)
	{
		norm.x = sin(vert.x * WaveTime * 0.002 + WaveTime) * 0.3;
		norm.z = cos(vert.z * WaveTime * 0.0008 + WaveTime);
		//norm.y = norm.x + norm.y;
		norm.y = 0.10f;
		//norm = vec3(sin(vert.x + WaveTime) * cos(vert.z + WaveTime) );
	}


	// Vertex normal in camera space
	//N = normalize(transpose(inverse(ModelMatrix)) * vec4(normal, 0)).xyz;
	N = normalize(transpose(inverse(ModelMatrix)) * vec4(norm, 0)).xyz;

	if(v.y <= 0)
	{
		N = vec3(0.0f, 1.0f, 0.0f);
	}
	//N = normalize(NormalMatrix * vec4(normal, 0)).xyz;
	//N = normalize(normal);

	// Calculate vertex position on screen
	//gl_Position = ModelMatrix * ViewMatrix * ProjectionMatrix * vec4(vertex, 1);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vert, 1.0f);
	//gl_Position = ProjectionMatrix * ViewMatrix * vec4(vertex, 1.0f);
	//gl_Position = ViewMatrix * vec4(vertex, 1.0f);
}