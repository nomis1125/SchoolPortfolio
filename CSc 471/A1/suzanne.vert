<<<<<<< HEAD
#version 450 core
//phong
#include "UniformLocations.glsl"

layout(location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout(location = NORMAL_ATTRIB_LOCATION) in vec3 normal;



layout (std140, binding = UNIFORM_MATRIX_LOCATION) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 model;
};

out vec3 vertexNormal;
out vec3 fragPos;
out vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

	fragPos = vec3(model * vec4(position, 1.0));

	viewPos = vec3(inverse(view) * vec4(0,0,0,1.0));

	vertexNormal = normal;
}
=======
#version 450 core
//phong
#include "UniformLocations.glsl"

layout(location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout(location = NORMAL_ATTRIB_LOCATION) in vec3 normal;



layout (std140, binding = UNIFORM_MATRIX_LOCATION) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 model;
};

out vec3 vertexNormal;
out vec3 fragPos;
out vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

	fragPos = vec3(model * vec4(position, 1.0));

	viewPos = vec3(inverse(view) * vec4(0,0,0,1.0));

	vertexNormal = normal;
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
