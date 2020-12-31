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

	vec3 c0RGB;
	vec3 c1RGB;
	vec3 v3Direction;//t0

	vec3 c00RGB;
	vec3 c01RGB;
} inData;

out vec4 fragColour;

//Mie
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
    return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(abs(1.0 + g2 - 2.0*g*fCos), 1.5);
}

//Rayleigh
float getRayleighPhase(float fCos2)
{
    return 0.75 + 0.75*fCos2;
}

void main()
{

    vec4 planet = texture(imageTexture, inData.texCoord);
	vec3 base = vec3(0.25, 0.25, 0.25);

    //OuterAtmo
    float g = -0.98f;
	float g2 = 0.9604f;

    float fCos = dot(inData.v3LightDir, inData.v3Direction) / length(inData.v3Direction);
	float fCos2 = fCos * fCos;
	vec3 colour = getRayleighPhase(fCos2) * inData.c0RGB + getMiePhase(fCos, fCos2, g, g2) * inData.c1RGB;

	//vec3 invColour = vec3(1.0 - colour.x, 1.0 - colour.y, 1.0 - colour.z);

	vec4 outAtmo = vec4(colour, 1.0f);

	fragColour = outAtmo;
    
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

	vec3 c0RGB;
	vec3 c1RGB;
	vec3 v3Direction;//t0

	vec3 c00RGB;
	vec3 c01RGB;
} inData;

out vec4 fragColour;

//Mie
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
    return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(abs(1.0 + g2 - 2.0*g*fCos), 1.5);
}

//Rayleigh
float getRayleighPhase(float fCos2)
{
    return 0.75 + 0.75*fCos2;
}

void main()
{

    vec4 planet = texture(imageTexture, inData.texCoord);
	vec3 base = vec3(0.25, 0.25, 0.25);

    //OuterAtmo
    float g = -0.98f;
	float g2 = 0.9604f;

    float fCos = dot(inData.v3LightDir, inData.v3Direction) / length(inData.v3Direction);
	float fCos2 = fCos * fCos;
	vec3 colour = getRayleighPhase(fCos2) * inData.c0RGB + getMiePhase(fCos, fCos2, g, g2) * inData.c1RGB;

	//vec3 invColour = vec3(1.0 - colour.x, 1.0 - colour.y, 1.0 - colour.z);

	vec4 outAtmo = vec4(colour, 1.0f);

	fragColour = outAtmo;
    
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
