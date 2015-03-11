#version 130

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 N;
in vec3 v;

uniform Material material;
uniform Light light;

out vec4 outputColor;


void main()
{
	// Light vector L
	vec3 L = normalize(light.position.xyz - v);

	// Camera vector C
	vec3 C = normalize(-v);

	// Reflection vector R
	vec3 R = normalize(-reflect(L,N));


	// Calculate color components
	vec3 ambient = material.ambient * light.ambient;
	vec3 diffuse = material.diffuse * light.diffuse * max(dot(N,L), 0.0);
	vec3 specular = material.specular * light.specular * pow(max(dot(R,C),0.0),material.shininess);
	specular = clamp(specular, 0.0, 1.0);

	// Calculate final color
	outputColor = vec4(ambient + diffuse + specular, 1);
	//outputColor = vec4(light.ambient, 1.0f);
	//outputColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
}