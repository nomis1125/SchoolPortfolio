<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (vertices = 3) out;

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices
{
    mat4 model;
    mat4 viewModel;
    mat4 MVP;
    mat4 invTModel;
};

uniform int inner;
uniform int outer;

in vec3 fragPosition[];

out vec3 tesc_to_tese[];

void main(){

	gl_TessLevelInner[0] = inner;

	gl_TessLevelOuter[0] = outer;
	gl_TessLevelOuter[1] = outer;
	gl_TessLevelOuter[2] = outer;


	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
	tesc_to_tese[gl_InvocationID] = fragPosition[gl_InvocationID];
=======
#version 450 core

#include "UniformLocations.glsl"

layout (vertices = 3) out;

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices
{
    mat4 model;
    mat4 viewModel;
    mat4 MVP;
    mat4 invTModel;
};

uniform int inner;
uniform int outer;

in vec3 fragPosition[];

out vec3 tesc_to_tese[];

void main(){

	gl_TessLevelInner[0] = inner;

	gl_TessLevelOuter[0] = outer;
	gl_TessLevelOuter[1] = outer;
	gl_TessLevelOuter[2] = outer;


	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
	tesc_to_tese[gl_InvocationID] = fragPosition[gl_InvocationID];
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}