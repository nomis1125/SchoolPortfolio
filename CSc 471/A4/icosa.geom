<<<<<<< HEAD
#version 450

#include "UniformLocations.glsl"
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices{
	mat4 model;
	mat4 viewModel;
	mat4 MVP;
	mat4 invTModel;
};


in vec3 p012Out[];
in vec3 tePatchDistance[];

out vec3 oUV; //distance from vertex to opposing edge
out vec3 tePatchDistance2; //triangle distance
out vec3 normal; //the normal of triangle

void main()
{

	vec3 A = p012Out[2] - p012Out[0];
	vec3 B = p012Out[1] - p012Out[0];


	mat3 m = mat3(invTModel);
	normal = m*normalize(cross(A,B));

	oUV = p012Out[0];
	tePatchDistance2 = vec3(1, 0, 0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	oUV = p012Out[1];
	tePatchDistance2 = vec3(0, 1, 0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	oUV = p012Out[2];
	tePatchDistance2 = vec3(0, 0, 1);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
=======
#version 450

#include "UniformLocations.glsl"
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = MATRIX_BUFFER_BINDING) uniform Matrices{
	mat4 model;
	mat4 viewModel;
	mat4 MVP;
	mat4 invTModel;
};


in vec3 p012Out[];
in vec3 tePatchDistance[];

out vec3 oUV; //distance from vertex to opposing edge
out vec3 tePatchDistance2; //triangle distance
out vec3 normal; //the normal of triangle

void main()
{

	vec3 A = p012Out[2] - p012Out[0];
	vec3 B = p012Out[1] - p012Out[0];


	mat3 m = mat3(invTModel);
	normal = m*normalize(cross(A,B));

	oUV = p012Out[0];
	tePatchDistance2 = vec3(1, 0, 0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	oUV = p012Out[1];
	tePatchDistance2 = vec3(0, 1, 0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	oUV = p012Out[2];
	tePatchDistance2 = vec3(0, 0, 1);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}