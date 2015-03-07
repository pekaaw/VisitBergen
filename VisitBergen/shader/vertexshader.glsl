#version 330

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

//uniform vec3 CameraPosition;
//uniform vec3 DirectionalLightDirection;

in vec3 vertex;
in vec2 tex_coords;
in vec3 vertex_normal;

//out vec2 tex_coords_to_fragment;
//out float diffuse_intensity;
//out float specular_intensity;
//out float light_distance;

out vec3 N;
out vec3 v;
out vec2 textureCoordinates;


void main()
{
	v = vec3(ViewMatrix * vec4(vertex, 1));
	N = normalize( NormalMatrix * vertex_normal);

	textureCoordinates = tex_coords;

//	vec3 normal = normalize(NormalMatrix * vertex_normal);
//	vec3 lightDirection = CameraPosition - position;
//	light_distance = length(lightDirection);
//	light_distance = light_distance * light_distance;
//	lightDirection = normalize(lightDirection);

//	vec4 lightDirection = normalize(DirectionalLightDirection - position);

//	float NdotL = dot(normal, lightDirection);
//	diffuse_intensity = clamp( NdotL, 0.0f, 1.0f );

//	vec3 H = normalize(DirectionalLightDirection + lightDirection);
//	float NdotH = dot(normal, H);
//	specular_intensity = clamp( NdotH, 0.0f, 1.0f );

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);

//	tex_coords_to_fragment = tex_coords;
}