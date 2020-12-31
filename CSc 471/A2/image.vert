<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;
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
    vec2 texCoord;
}outData;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);

    outData.position = position;
    outData.texCoord = texCoord;
}
=======
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;
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
    vec2 texCoord;
}outData;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);

    outData.position = position;
    outData.texCoord = texCoord;
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
