<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout (location = NORMAL_ATTRIB_LOCATION) in vec3 normal;
layout (location = TEXTURE_ATTRIB_LOCATION) in vec2 texCoord;

layout (binding = UNIFORM_MATRIX_LOCATION) uniform Matrices
{
    mat4 model;
    mat4 viewModel;
    mat4 MVP;
    mat4 invTModel;
};

out VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} outData;

out vec3 vertexNormal;
out vec3 fragPos;
out vec3 viewPos;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);

    outData.position = position;
    outData.normal = normalize(invTModel * vec4(normal, 0.0)).xyz;
    outData.texCoord = texCoord;

	fragPos = vec3(model * vec4(position, 1.0));
	viewPos = vec3(inverse(viewModel) * vec4(0,0,0,1.0));
	vertexNormal = normal;
}
=======
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout (location = NORMAL_ATTRIB_LOCATION) in vec3 normal;
layout (location = TEXTURE_ATTRIB_LOCATION) in vec2 texCoord;

layout (binding = UNIFORM_MATRIX_LOCATION) uniform Matrices
{
    mat4 model;
    mat4 viewModel;
    mat4 MVP;
    mat4 invTModel;
};

out VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} outData;

out vec3 vertexNormal;
out vec3 fragPos;
out vec3 viewPos;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);

    outData.position = position;
    outData.normal = normalize(invTModel * vec4(normal, 0.0)).xyz;
    outData.texCoord = texCoord;

	fragPos = vec3(model * vec4(position, 1.0));
	viewPos = vec3(inverse(viewModel) * vec4(0,0,0,1.0));
	vertexNormal = normal;
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
