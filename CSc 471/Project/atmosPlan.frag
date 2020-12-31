<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (binding = UNIFORM_TEXTURE_LOCATION) uniform sampler2D imageTexture;

in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;

	vec3 v3LightDir;

	vec3 c00RGB;
	vec3 c01RGB;
} inData;

out vec4 fragColour;

void main()
{

    vec4 planet = texture(imageTexture, inData.texCoord);
	vec3 base = vec3(0.25, 0.25, 0.25);
	
	//InnerAtmo
	vec3 equ = inData.c00RGB + base * inData.c01RGB;
	vec4 innerAtmo = vec4(equ, 1);

	fragColour = innerAtmo;// + outAtmo;
    
}
=======
#version 450 core

#include "UniformLocations.glsl"

layout (binding = UNIFORM_TEXTURE_LOCATION) uniform sampler2D imageTexture;

in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;

	vec3 v3LightDir;

	vec3 c00RGB;
	vec3 c01RGB;
} inData;

out vec4 fragColour;

void main()
{

    vec4 planet = texture(imageTexture, inData.texCoord);
	vec3 base = vec3(0.25, 0.25, 0.25);
	
	//InnerAtmo
	vec3 equ = inData.c00RGB + base * inData.c01RGB;
	vec4 innerAtmo = vec4(equ, 1);

	fragColour = innerAtmo;// + outAtmo;
    
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
