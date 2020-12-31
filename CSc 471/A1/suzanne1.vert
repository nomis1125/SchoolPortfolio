<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout(location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout(location = NORMAL_ATTRIB_LOCATION) in vec3 normal;

layout (std140, binding = UNIFORM_MATRIX_LOCATION) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 model;
};

out vec3 trueColour;

void main()
{

	vec3 lightPosition = vec3(10.0, 10.0, 10.0);

    gl_Position = projection * view * model * vec4(position, 1.0);

	vec3 fragPos = vec3(model * vec4(position, 1.0));

	vec3 viewPos = vec3(inverse(view) * vec4(0,0,0,1.0));
    
	//ambient

	vec3 lightColour = vec3(1.0, 1.0, 1.0);

	float ambientFactor = 0.1;

	vec3 colourOfObject = vec3(0, 0.46, 0.69);

	trueColour = ambientFactor * lightColour * colourOfObject;

	//diffuse

	float diffuseConstant = 0.5;

	vec3 lightDirection = normalize(lightPosition - fragPos);
	vec3 lightDirectionNon = lightPosition - fragPos;

	float diffuse = max(0.0, dot(normal, lightDirection));

	trueColour += (diffuseConstant * lightColour * diffuse);



	//specular

	float specularCoefficient = 0.9;

	float n = 32;

	vec3 viewDir = normalize(lightDirectionNon + viewPos);

	float spec = pow(max(dot(viewDir, normal), 0.0), n);
	vec3 specular = specularCoefficient * spec * lightColour;
	trueColour += specular;

	//final

	float emmisionCoefficient = 1.5;

	trueColour = trueColour * emmisionCoefficient * colourOfObject;

=======
#version 450 core

#include "UniformLocations.glsl"

layout(location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout(location = NORMAL_ATTRIB_LOCATION) in vec3 normal;

layout (std140, binding = UNIFORM_MATRIX_LOCATION) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 model;
};

out vec3 trueColour;

void main()
{

	vec3 lightPosition = vec3(10.0, 10.0, 10.0);

    gl_Position = projection * view * model * vec4(position, 1.0);

	vec3 fragPos = vec3(model * vec4(position, 1.0));

	vec3 viewPos = vec3(inverse(view) * vec4(0,0,0,1.0));
    
	//ambient

	vec3 lightColour = vec3(1.0, 1.0, 1.0);

	float ambientFactor = 0.1;

	vec3 colourOfObject = vec3(0, 0.46, 0.69);

	trueColour = ambientFactor * lightColour * colourOfObject;

	//diffuse

	float diffuseConstant = 0.5;

	vec3 lightDirection = normalize(lightPosition - fragPos);
	vec3 lightDirectionNon = lightPosition - fragPos;

	float diffuse = max(0.0, dot(normal, lightDirection));

	trueColour += (diffuseConstant * lightColour * diffuse);



	//specular

	float specularCoefficient = 0.9;

	float n = 32;

	vec3 viewDir = normalize(lightDirectionNon + viewPos);

	float spec = pow(max(dot(viewDir, normal), 0.0), n);
	vec3 specular = specularCoefficient * spec * lightColour;
	trueColour += specular;

	//final

	float emmisionCoefficient = 1.5;

	trueColour = trueColour * emmisionCoefficient * colourOfObject;

>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}