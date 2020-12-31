<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices
{
    mat4 model;
    mat4 viewModel;
    mat4 MVP;
    mat4 invTModel;
};

out vec3 fragPosition;

void main(){
	//gl_Position = MVP * vec4(position, 1.0);
	fragPosition = position;
=======
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices
{
    mat4 model;
    mat4 viewModel;
    mat4 MVP;
    mat4 invTModel;
};

out vec3 fragPosition;

void main(){
	//gl_Position = MVP * vec4(position, 1.0);
	fragPosition = position;
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}