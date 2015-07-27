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
in float isSea;

// Sampler to access texture
uniform sampler2D TextureSampler;
uniform bool NoTexture;

uniform Material material;
uniform Light headLight;
uniform Light directionalLight;
uniform Light movingLight;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform float WaveTime;

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
//	if( !NoTexture )
//	{
//		headSpecular = vec3(0.0f);
//	}
	
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
		vec3 R_movingLight = normalize(reflect(movingLightDirection, N));
		float movingLight_RdotL = max(dot(-movingLightDirection, R_movingLight), 0);

		float cutoffThreshold = 0.00001;
		float dmax = movingLight.lightRadius * (sqrt(movingLight.intensities.y / cutoffThreshold) - 1);
		float distanceRatio = movingLightDistance / dmax;
		float d = movingLightDistance / (1 - distanceRatio * distanceRatio);
		float r = movingLight.lightRadius;
		float attenuation = 1 / ((d/r + 1) * (d/r + 1));
		attenuation = (attenuation - cutoffThreshold) / (1 - cutoffThreshold);
		attenuation = max(attenuation, 0);
		attenuation /= 2;
		//attenuation = 1 / (1 + movingLightDistance * movingLightDistance);
		//attenuation = movingLightDistance;

	//vec3 movAmbient = movingLight.ambient * movingLight.intensities.x * attenuation;
	//vec3 movDiffuse = movingLight.diffuse * movingLight_NdotL * movingLight.intensities.y * attenuation;
	//vec3 movSpecular = clamp(movingLight.specular * movingLight.intensities.z * attenuation * movingLight_RdotL, 0.0f, 1.0f) * 0.5f;
	vec3 movAmbient = movingLight.ambient * movingLight.intensities.x;
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


	// Do gamma correction
	vec4 gamma = vec4(1.0/2.2);
	vec4 finalColor = vec4(ambient,1) + diffuse + vec4(specular, 1);
	//finalColor = pow(finalColor, gamma);

	if(isSea > 0.9f)
	{
		float factor = clamp(cos(v.y * WaveTime), 0.0f, 0.5f);
		//finalColor.xyz *= 0.5; // + factor;
//		finalColor.xyz += vec3(0.0f, 0.0f, 1.0f) * factor;
//		finalColor.xyz *= 0.5 + clamp(cos(v.y * WaveTime), 0.0f, 1.0f);
//		finalColor.xyz *= 0.5 + vec3(0.1f, 0.5f, 1.0f) * sin(v.y * WaveTime);
//		outputColor.w = sin(v.y * WaveTime);
		if(v.y > 0.1f)
			finalColor *= 1 - vec4(0.6f, 0.82f, 0.8f, 1.0f);
				//153:208:205
	//	else if(v.y > -0.01f)
	//		finalColor = vec4(0.0f, 0.55f, 0.73f, 1.0f);
				//0:141:186
		//else
		else if(v.y < -0.1f)
		finalColor *= vec4(0.0f, 0.41f, 0.58f, 1.0f);
	//			//0:105:148
	}

	// Calculate final color
	//outputColor = vec4(ambient,1) + diffuse + vec4(specular, 1);
	outputColor = pow(finalColor, gamma);

	//outputColor = diffuse + vec4(specular, 1);
	//outputColor = vec4(headAmbient, 1.0f);
	//outputColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
}