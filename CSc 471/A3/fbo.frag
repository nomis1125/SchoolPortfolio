<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

in vec2 texCoords;

out vec4 fragColour;

layout(binding = IMAGE_TEXTURE_BINDING) uniform sampler2D imageTextures;

mat3 sx = mat3(-1.0, 0.0, 1.0,
			   -2.0, 0.0, 2.0,
			   -1.0, 0.0, 1.0);

mat3 sy = mat3(-1.0, -2.0, -1.0,
			   0.0, 0.0, 0.0,
			   1.0, 2.0, 1.0);

void main()
{
	
	vec3 smplk00 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(-1, -1), 0).rgb;
	float smpl00 = dot(smplk00, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk01 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(0, -1), 0).rgb;
	float smpl01 = dot(smplk01, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk02 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(1, -1), 0).rgb;
	float smpl02 = dot(smplk02, vec3(0.2126, 0.7152, 0.0722));//luma x

	
	vec3 smplk10 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(-1, 0), 0).rgb;
	float smpl10 = dot(smplk10, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk11 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(0, 0), 0).rgb;
	float smpl11 = dot(smplk11, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk12 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(1, 0), 0).rgb;
	float smpl12 = dot(smplk12, vec3(0.2126, 0.7152, 0.0722));//luma x

	
	vec3 smplk20 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(-1, 1), 0).rgb;
	float smpl20 = dot(smplk20, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk21 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(0, 1), 0).rgb;
	float smpl21 = dot(smplk21, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk22 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(1, 1), 0).rgb;
	float smpl22 = dot(smplk22, vec3(0.2126, 0.7152, 0.0722));//luma x


	float sx2v3 = (sx[2][2] * smpl00 + sx[2][1] * smpl01 + sx[2][0] * smpl02 + sx[1][2] * smpl10 + sx[1][1] * smpl11 + sx[1][0] * smpl12 + sx[0][2] *smpl20 + sx[0][1] * smpl21 + sx[0][0] * smpl22);
	
	float sy2v3 = (sy[2][2] * smpl00 + sy[2][1] * smpl01 + sy[2][0] * smpl02 + sy[1][2] * smpl10 + sy[1][1] * smpl11 + sy[1][0] * smpl12 + sy[0][2] * smpl20 + sy[0][1] *smpl21 + sy[0][0] * smpl22);

	

	float g = length(vec2(sx2v3, sy2v3));

	float gk = max(g, 0.01);

	fragColour = vec4(vec3(gk), 1.0);
}
=======
#version 450 core

#include "UniformLocations.glsl"

in vec2 texCoords;

out vec4 fragColour;

layout(binding = IMAGE_TEXTURE_BINDING) uniform sampler2D imageTextures;

mat3 sx = mat3(-1.0, 0.0, 1.0,
			   -2.0, 0.0, 2.0,
			   -1.0, 0.0, 1.0);

mat3 sy = mat3(-1.0, -2.0, -1.0,
			   0.0, 0.0, 0.0,
			   1.0, 2.0, 1.0);

void main()
{
	
	vec3 smplk00 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(-1, -1), 0).rgb;
	float smpl00 = dot(smplk00, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk01 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(0, -1), 0).rgb;
	float smpl01 = dot(smplk01, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk02 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(1, -1), 0).rgb;
	float smpl02 = dot(smplk02, vec3(0.2126, 0.7152, 0.0722));//luma x

	
	vec3 smplk10 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(-1, 0), 0).rgb;
	float smpl10 = dot(smplk10, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk11 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(0, 0), 0).rgb;
	float smpl11 = dot(smplk11, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk12 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(1, 0), 0).rgb;
	float smpl12 = dot(smplk12, vec3(0.2126, 0.7152, 0.0722));//luma x

	
	vec3 smplk20 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(-1, 1), 0).rgb;
	float smpl20 = dot(smplk20, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk21 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(0, 1), 0).rgb;
	float smpl21 = dot(smplk21, vec3(0.2126, 0.7152, 0.0722));//luma x

	vec3 smplk22 = texelFetch(imageTextures, ivec2(gl_FragCoord) + ivec2(1, 1), 0).rgb;
	float smpl22 = dot(smplk22, vec3(0.2126, 0.7152, 0.0722));//luma x


	float sx2v3 = (sx[2][2] * smpl00 + sx[2][1] * smpl01 + sx[2][0] * smpl02 + sx[1][2] * smpl10 + sx[1][1] * smpl11 + sx[1][0] * smpl12 + sx[0][2] *smpl20 + sx[0][1] * smpl21 + sx[0][0] * smpl22);
	
	float sy2v3 = (sy[2][2] * smpl00 + sy[2][1] * smpl01 + sy[2][0] * smpl02 + sy[1][2] * smpl10 + sy[1][1] * smpl11 + sy[1][0] * smpl12 + sy[0][2] * smpl20 + sy[0][1] *smpl21 + sy[0][0] * smpl22);

	

	float g = length(vec2(sx2v3, sy2v3));

	float gk = max(g, 0.01);

	fragColour = vec4(vec3(gk), 1.0);
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
