#version 150

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
	vec3 intensities;
};


in vec3 N;
in vec3 v;

uniform Material material;
uniform Light headLight;
uniform Light directionalLight;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

out vec4 outputColor;


void main()
{
	// Camera vector C
	vec3 camera_position = (inverse(ViewMatrix) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
	vec3 C = normalize( camera_position - v );

	// Light vector L
	//vec3 L = normalize(light.position.xyz - v);
	vec3 L_head = C;
	vec3 L_dir = normalize(directionalLight.position.xyz - v);

	// Reflection vector R
	//vec3 R = normalize(reflect(L,N));
	vec3 R_head = normalize(reflect(L_head,N));
	vec3 R_dir = normalize(reflect(L_dir,N));

	// Half vector between light and camera
	//vec3 H = normalize( L + C );

	float NdotL = clamp(dot( N, L_head ), 0.0f, 1.0f);
	float RdotL = clamp(dot( -L_head, R_head ), 0.0f, 1.0f);

	float NdotLdir = clamp(dot(N,L_dir), 0.0f, 1.0f);
	float RdotLdir = clamp(dot(R_dir, -L_dir), 0.0f, 1.0f);


	// head Light;
	vec3 headAmbient = headLight.ambient * headLight.intensities.x;
	vec3 headDiffuse = headLight.diffuse * headLight.intensities.y * NdotL;
	//vec3 headSpecular = headLight.specular * headLight.intensities.z * pow(max(0.0, dot(R_head,C)), 1); // * material.shininess);
	vec3 headSpecular = headLight.specular * headLight.intensities.z * pow((RdotL), 50); // * material.shininess);
	
	// directian Light
	vec3 dirAmbient = directionalLight.ambient * directionalLight.intensities.x;
	vec3 dirDiffuse = directionalLight.diffuse * directionalLight.intensities.y * NdotLdir;
	vec3 dirSpecular = directionalLight.specular * directionalLight.intensities.z * pow(RdotLdir, 25); // pow(max(0.01, dot(R_dir,C)), 50); // * material.shininess);

	// total values
	vec3 ambient = (material.ambient * headAmbient) + (material.ambient * dirAmbient);
	vec3 diffuse = (material.diffuse * headDiffuse) + (material.diffuse * dirDiffuse);
	vec3 specular = (material.specular * clamp(headSpecular, 0.0f, 1.0f)) + (material.specular * clamp(dirSpecular, 0.01f, 1.0f));

	// Calculate final color
	outputColor = vec4(ambient + diffuse + specular, 1);
	//outputColor = vec4(headAmbient, 1.0f);
	//outputColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
}