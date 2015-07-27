#version 330

in vec3 TexCoord0;

out vec4 FragColor;

uniform samplerCube CubeMapTexture;


void main()
{
	FragColor = texture(CubeMapTexture, TexCoord0);
}