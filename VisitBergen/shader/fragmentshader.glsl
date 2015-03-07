#version 330

// Sampler to access texture
uniform sampler2D TextureSampler;

//uniform vec3 DirectionalLightAmbientColor;
//uniform vec3 DirectionalLightDiffuseColor;
//uniform vec3 DirectionalLightSpecularColor;

//uniform float DirectionalLightAmbientIntensity;
//uniform float DirectionalLightDiffuseIntensity;
//uniform float DirectionalLightSpecularIntensity;

//uniform vec4 MaterialAmbientColor;
//uniform vec4 MaterialDiffuseColor;
//uniform vec4 MaterialSpecularColor;
//uniform float MaterialShininess;
//uniform float MaterialAlpha;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light
{
	vec4 position; // w=0 directional light, w=1 point light
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

uniform bool NoTexture;

// Per vertex texture coordinates
//in vec2 tex_coords_to_fragment;
//in float diffuse_intensity;
//in float specular_intensity;
//in float light_distance;

in vec3 N;
in vec3 v;
in vec2 textureCoordinates;


out vec4 out_color;

void main()
{
	//out_color = texture2D(TextureSampler, vec2(tex_coords_to_fragment.s, 1.0 - tex_coords_to_fragment.t));

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	
	vec3 L = normalize(light.position.xyz - v);
	vec3 E = normalize(-v);
	vec3 R = normalize(-reflect(L,N));

	ambientColor = material.ambient * light.ambient;
	//ambientColor = MaterialAmbientColor * vec4(DirectionalLightAmbientColor, 1.0f) * DirectionalLightAmbientIntensity;

	if(NoTexture)
	{
		//diffuseColor = diffuse_intensity * MaterialDiffuseColor * vec4(DirectionalLightDiffuseColor, 1.0f) * DirectionalLightDiffuseIntensity / light_distance;
		diffuseColor = material.diffuse * light.diffuse * max(dot(N,L), 0.0);
	}
	else
	{
		vec4 textureColor = texture2D(TextureSampler, vec2(textureCoordinates.s, 1.0 - textureCoordinates.t));
		diffuseColor = textureColor * light.diffuse * max(dot(N,L), 0.0);
		//diffuseColor = texture2D(TextureSampler, vec2(tex_coords_to_fragment.s, 1.0 - tex_coords_to_fragment.t));
	}

	//float spec = pow(specular_intensity, MaterialShininess);
	//specularColor = spec * MaterialSpecularColor * vec4(DirectionalLightSpecularColor, 1.0f) * DirectionalLightSpecularIntensity / light_distance;

	specularColor = material.specular * light.specular * pow(max(dot(R,E), 0.0), 0.3 * material.shininess);

	out_color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}