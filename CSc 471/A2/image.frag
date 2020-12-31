<<<<<<< HEAD
#version 450 core

#include "UniformLocations.glsl"

in VertexData
{
    vec3 position;
    vec2 texCoord;
} inData;

layout (binding = UNIFORM_TEXTURE_LOCATION) uniform sampler2D imageTexture;//original

layout (binding = SOMETHING_ELSE) uniform sampler3D imageBexture;//the 3d normal texture
layout (binding = SOMETHING_ELSEInv) uniform sampler3D imageBextureInv;//the 3d invert texture
layout (binding = SOMETHING_ELSEBW) uniform sampler3D imageBextureBW;//the 3d black white texture

uniform int LUTmode;

out vec4 otherColour;

void main()
{
    //fragColour = vec4(1, 1, 1, 1);
	
	vec4 fragColour = texture(imageTexture, inData.texCoord);

	float s = 10.0;//sizeOfLUT; 10 works

	vec3 bacon = fragColour.xyz;
	
	vec3 otherColour2 = (((s-1.0)/s)*bacon)+(1.0/(2.0*s));
	
	if(LUTmode == 2){
		otherColour = texture(imageBextureBW, otherColour2);
	}
	else if(LUTmode == 1){
		otherColour = texture(imageBextureInv, otherColour2);
	}
	else{
		otherColour = texture(imageBexture, otherColour2);
	}
}
=======
#version 450 core

#include "UniformLocations.glsl"

in VertexData
{
    vec3 position;
    vec2 texCoord;
} inData;

layout (binding = UNIFORM_TEXTURE_LOCATION) uniform sampler2D imageTexture;//original

layout (binding = SOMETHING_ELSE) uniform sampler3D imageBexture;//the 3d normal texture
layout (binding = SOMETHING_ELSEInv) uniform sampler3D imageBextureInv;//the 3d invert texture
layout (binding = SOMETHING_ELSEBW) uniform sampler3D imageBextureBW;//the 3d black white texture

uniform int LUTmode;

out vec4 otherColour;

void main()
{
    //fragColour = vec4(1, 1, 1, 1);
	
	vec4 fragColour = texture(imageTexture, inData.texCoord);

	float s = 10.0;//sizeOfLUT; 10 works

	vec3 bacon = fragColour.xyz;
	
	vec3 otherColour2 = (((s-1.0)/s)*bacon)+(1.0/(2.0*s));
	
	if(LUTmode == 2){
		otherColour = texture(imageBextureBW, otherColour2);
	}
	else if(LUTmode == 1){
		otherColour = texture(imageBextureInv, otherColour2);
	}
	else{
		otherColour = texture(imageBexture, otherColour2);
	}
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
