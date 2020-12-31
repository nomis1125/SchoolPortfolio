<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (triangles, equal_spacing, cw) in; //??

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices{
	mat4 model;
	mat4 viewModel;
	mat4 MVP;
	mat4 invTModel;
};

in vec3 tesc_to_tese[];

out vec3 p012Out;
out vec3 tePatchDistance;

void main(){
	vec3 p0 = tesc_to_tese[0] * gl_TessCoord.x;
	vec3 p1 = tesc_to_tese[1] * gl_TessCoord.y;
	vec3 p2 = tesc_to_tese[2] * gl_TessCoord.z;
	
	vec3 p012 = normalize(p0 + p1 + p2);

	p012Out = p012;//??

	tePatchDistance = gl_TessCoord;
	gl_Position = MVP * vec4(p012, 1.0);
=======
#version 450 core

#include "UniformLocations.glsl"

layout (triangles, equal_spacing, cw) in; //??

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices{
	mat4 model;
	mat4 viewModel;
	mat4 MVP;
	mat4 invTModel;
};

in vec3 tesc_to_tese[];

out vec3 p012Out;
out vec3 tePatchDistance;

void main(){
	vec3 p0 = tesc_to_tese[0] * gl_TessCoord.x;
	vec3 p1 = tesc_to_tese[1] * gl_TessCoord.y;
	vec3 p2 = tesc_to_tese[2] * gl_TessCoord.z;
	
	vec3 p012 = normalize(p0 + p1 + p2);

	p012Out = p012;//??

	tePatchDistance = gl_TessCoord;
	gl_Position = MVP * vec4(p012, 1.0);
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}