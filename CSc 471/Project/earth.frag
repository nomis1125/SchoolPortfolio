<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (binding = UNIFORM_TEXTURE_LOCATION) uniform sampler2D imageTexture;

in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} inData;

in vec3 vertexNormal;
in vec3 fragPos;
in vec3 viewPos;

out vec4 fragColour;

void main()
{
    /*
    vec3 lightPosition = vec3(100.0, 100.0, 100.0); 

	vec3 trueColour;

	//ambient

	vec3 lightColour = vec3(1.0, 1.0, 1.0);

	float ambientFactor = 0.1;

	vec3 colourOfObject = vec3(texture(imageTexture, inData.texCoord));

	trueColour = (ambientFactor * lightColour * colourOfObject);

	//diffuse

	float diffuseConstant = 0.5;

	vec3 lightDirection = normalize(lightPosition - fragPos);
	vec3 lightDirectionNon = lightPosition - fragPos;

	float diffuse = max(0.0, dot(vertexNormal, lightDirection));

	trueColour += (diffuseConstant * lightColour * diffuse);



	//specular

	float specularCoefficient = 0.9;

	float n = 32;

	vec3 viewDir = normalize(lightDirectionNon + viewPos);

	float spec = pow(max(dot(viewDir, vertexNormal), 0.0), n);
	vec3 specular = specularCoefficient * spec * lightColour;
	trueColour += specular;

	//final

	float emmisionCoefficient = 1.5;

	trueColour = clamp(trueColour * emmisionCoefficient, 0.0, 1.0);

    fragColour = vec4(trueColour, 1.0) * texture(imageTexture, inData.texCoord);
    */
    fragColour = texture(imageTexture, inData.texCoord);
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
} inData;

in vec3 vertexNormal;
in vec3 fragPos;
in vec3 viewPos;

out vec4 fragColour;

void main()
{
    /*
    vec3 lightPosition = vec3(100.0, 100.0, 100.0); 

	vec3 trueColour;

	//ambient

	vec3 lightColour = vec3(1.0, 1.0, 1.0);

	float ambientFactor = 0.1;

	vec3 colourOfObject = vec3(texture(imageTexture, inData.texCoord));

	trueColour = (ambientFactor * lightColour * colourOfObject);

	//diffuse

	float diffuseConstant = 0.5;

	vec3 lightDirection = normalize(lightPosition - fragPos);
	vec3 lightDirectionNon = lightPosition - fragPos;

	float diffuse = max(0.0, dot(vertexNormal, lightDirection));

	trueColour += (diffuseConstant * lightColour * diffuse);



	//specular

	float specularCoefficient = 0.9;

	float n = 32;

	vec3 viewDir = normalize(lightDirectionNon + viewPos);

	float spec = pow(max(dot(viewDir, vertexNormal), 0.0), n);
	vec3 specular = specularCoefficient * spec * lightColour;
	trueColour += specular;

	//final

	float emmisionCoefficient = 1.5;

	trueColour = clamp(trueColour * emmisionCoefficient, 0.0, 1.0);

    fragColour = vec4(trueColour, 1.0) * texture(imageTexture, inData.texCoord);
    */
    fragColour = texture(imageTexture, inData.texCoord);
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
