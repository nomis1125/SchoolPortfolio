<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout (location = TEXTURE_ATTRIB_LOCATION) in vec2 tex;

out vec2 texCoords;

void main()
{
	gl_Position = vec4(position, 1.0);

	texCoords = tex;
=======
#version 450 core

#include "UniformLocations.glsl"

layout (location = VERTEX_ATTRIB_LOCATION) in vec3 position;
layout (location = TEXTURE_ATTRIB_LOCATION) in vec2 tex;

out vec2 texCoords;

void main()
{
	gl_Position = vec4(position, 1.0);

	texCoords = tex;
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}