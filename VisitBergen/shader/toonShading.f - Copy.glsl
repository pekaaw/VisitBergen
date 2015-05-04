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
	float lightRadius;
	float litRadius;
//	bool dirLight;
};

//uniform Light lights[20];
//int number_of_lights;
// When wanting multiple lights, it is ok to set it as an array like above and
// using an int as a counter for how many to use. Values can be transferred as
// usual with "lights[0].position" and "lights[1].position" etc..

in vec3 N;
in vec3 v;
in vec2 textureCoordinates;

// Sampler to access texture
uniform sampler2D TextureSampler;
uniform bool NoTexture;

uniform Material material;
uniform Light headLight;
uniform Light directionalLight;
uniform Light movingLight;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

out vec4 outputColor;


void main()
{
	// Camera vector
	vec3 camera_position = (inverse(ViewMatrix) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
	vec3 camera_vector = normalize( camera_position - v );

	// Light vector L
	vec3 L_head = camera_vector;
	vec3 L_dir = normalize(directionalLight.position.xyz - v);

	// Reflection vector R
	vec3 R_head = normalize(reflect(L_head,N));
	vec3 R_dir = normalize(reflect(L_dir,N));

	// Half vector between light and camera
	//vec3 H = normalize( L + camera_vector );

	float NdotL = clamp(dot( N, L_head ), 0.0f, 1.0f);
	float RdotL = clamp(dot( -L_head, R_head ), 0.0f, 1.0f);

	float NdotLdir = clamp(dot(N,L_dir), 0.0f, 1.0f);
	float RdotLdir = clamp(dot(R_dir, -L_dir), 0.0f, 1.0f);

	if( NdotL > 0.95 )
		NdotL = 1.0;
	else if ( NdotL > 0.5 )
		NdotL = 0.7;
	else if ( NdotL > 0.25 )
		NdotL = 0.5;
	else
		NdotL = 0.2;

	if( NdotLdir > 0.95 )
		NdotLdir = 1.0;
	else if ( NdotLdir > 0.5 )
		NdotLdir = 0.7;
	else if ( NdotLdir > 0.25 )
		NdotLdir = 0.5;
	else
		NdotLdir = 0.2;

	// head Light;
	vec3 headAmbient = headLight.ambient * headLight.intensities.x;
	vec3 headDiffuse = headLight.diffuse * headLight.intensities.y * NdotL;
	vec3 headSpecular = headLight.specular * headLight.intensities.z * pow((RdotL), 50); // * material.shininess);

	
	// directian Light
	vec3 dirAmbient = directionalLight.ambient * directionalLight.intensities.x;
	vec3 dirDiffuse = directionalLight.diffuse * directionalLight.intensities.y * NdotLdir;
	vec3 dirSpecular = directionalLight.specular * directionalLight.intensities.z * pow(RdotLdir, 25); // pow(max(0.01, dot(R_dir,C)), 50); // * material.shininess);

	// moving point light - from https://imdoingitwrong.wordpress.com/2011/02/10/improved-light-attenuation/
		vec3 movingLightDirection = movingLight.position.xyz - v;
		float movingLightDistance = length(movingLightDirection);
		movingLightDistance = max(movingLightDistance - movingLight.lightRadius, 0);
		movingLightDirection /= movingLightDistance;	// normalization

		float movingLight_NdotL = max(dot(movingLightDirection, N), 0);

		if ( movingLight_NdotL > 0.95 )
			movingLight_NdotL = 1.0;
		else if ( movingLight_NdotL > 0.5 )
			movingLight_NdotL = 0.7;
		else if ( movingLight_NdotL > 0.25 )
			movingLight_NdotL = 0.5;
		else
			movingLight_NdotL = 0.2;

		vec3 R_movingLight = normalize(reflect(movingLightDirection, N));
		float movingLight_RdotL = max(dot(-movingLightDirection, R_movingLight), 0);

		float distanceRatio = movingLightDistance / movingLight.litRadius;
		float d = movingLightDistance / (1 - distanceRatio * distanceRatio);
		float r = movingLight.lightRadius;
		float attenuation = 1 / ((d/r + 1) * (d/r + 1));

	vec3 movAmbient = movingLight.ambient * attenuation;
	vec3 movDiffuse = movingLight.diffuse * movingLight_NdotL * attenuation;
	vec3 movSpecular = clamp(movingLight.specular * attenuation * movingLight_RdotL, 0.0f, 1.0f) * 0.5f;

	// Texture values
	vec4 texColor = texture2D(TextureSampler, vec2(textureCoordinates.s, 1.0 - textureCoordinates.t));

	// total values
	//vec3 ambient = (material.ambient * headAmbient) + (material.ambient * dirAmbient) + (material.ambient * movAmbient);
	//vec3 diffuse = (material.diffuse * headDiffuse) + (material.diffuse * dirDiffuse) + (material.diffuse * movDiffuse);
	//vec3 specular = (material.specular * clamp(headSpecular, 0.0f, 1.0f)) + (material.specular * clamp(dirSpecular, 0.01f, 1.0f)) + (material.specular * movSpecular);
	vec3 ambient = (material.ambient * headAmbient) + (material.ambient * dirAmbient) + (material.ambient * movAmbient);
	vec4 diffuse;
	if( NoTexture) 
	{
		diffuse = vec4((material.diffuse * headDiffuse) + (material.diffuse * dirDiffuse) + (material.diffuse * movDiffuse), 1.0);
	}
	else
	{
		diffuse = (texColor * vec4(headDiffuse,1)) + (texColor * vec4(dirDiffuse,1)) + (texColor * vec4(movDiffuse,1));
	}

	vec3 specular = (material.specular * clamp(headSpecular, 0.0f, 1.0f)) + (material.specular * clamp(dirSpecular, 0.01f, 1.0f)) + (material.specular * movSpecular);

	// Calculate final color

	// Toon shading outline (http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Toon_Shading#Outlines)
	if( dot(camera_vector, normalize(N)) < 0.2 )
		outputColor = vec4(0, 0, 0, 1);
	else
		outputColor = vec4(ambient,1) + diffuse + vec4(specular, 1);
	//outputColor = diffuse;


}