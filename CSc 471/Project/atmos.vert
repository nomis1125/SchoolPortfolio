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

	vec3 v3LightDir;

	vec3 c0RGB;
	vec3 c1RGB;
	vec3 v3Direction;//t0

	vec3 c00RGB;
	vec3 c01RGB;
} outData;

float getNearIntersection(vec3 v3Pos, vec3 v3Ray, float fDistance2, float fRadius2)
{
    float B = 2.0 * dot(v3Pos, v3Ray);
    float C = fDistance2 - fRadius2;
    float fDet = max(0.0, B*B - 4.0 * C);
    return 0.5 * (-B - sqrt(fDet));
}
//Scale
float scale(float fCos, float fScaleDepth)
{
    float x = 1.0 - fCos;
    return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main()
{
    gl_Position = MVP * vec4(position, 1.0);

    outData.position = position;
    outData.normal = normalize(invTModel * vec4(normal, 0.0)).xyz;
    outData.texCoord = texCoord;

	//Atmosphere Edge Code

	float pi = 3.1415926535897932384626433832795f;

	float Kr = 0.0015f;//ray
	float Km = 0.0025f;//mie
	float ESun = 3.0f;
	int nSamples = 4;
	float fSamples = 4.0f;
	vec3 v3CameraPos = vec3(inverse(viewModel) * vec4(0.0f,0.0f,0.0f,1.0f));

	vec3 v3LightDir = vec3(100.0f, 100.0f, 100.0f);
	v3LightDir = normalize(v3LightDir);
	outData.v3LightDir = v3LightDir;
	vec3 v3InvWavelength = vec3(1.0f/pow(0.650f, 4),1.0f/pow(0.570f, 4),1.0f/pow(0.475f, 4));
	float fCameraHeight = sqrt(v3CameraPos.x*v3CameraPos.x+v3CameraPos.y*v3CameraPos.y+v3CameraPos.z*v3CameraPos.z);

	float fCameraHeight2 = fCameraHeight * fCameraHeight;
	float fOuterRadius = 2.050f;
	float fOuterRadius2 = fOuterRadius * fOuterRadius;

	float fInnerRadius = 2.0f;
	float fInnerRadius2 = fInnerRadius * fInnerRadius;
	float fKrESun = Kr * ESun;

	float fKmESun = Km * ESun;
	float fKr4PI = Kr * 4.0f * pi;
	float fKm4PI = Km * 4.0f * pi;

	float fScaleDepth = 0.25f;
	float fInvScaleDepth = 1.0f/fScaleDepth;

	float fScale = 1.0f/(fOuterRadius - fInnerRadius);
	float fScaleOverScaleDepth = fScale/fScaleDepth;

	//mathing
	vec3 v3Pos = position.xyz;
	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	float fNear = getNearIntersection(v3CameraPos, v3Ray, fCameraHeight2, fOuterRadius2);

	vec3 v3Start = v3CameraPos + v3Ray * fNear;//A
	fFar -= fNear;
	float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
	float fStartDepth = exp(-fInvScaleDepth);
	float fStartOffset = fStartDepth * scale(fStartAngle, fScaleDepth);

	float fSampleLength = fFar/fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5f;

	vec3 v3FrontColour = vec3(0.0f, 0.0f, 0.0f);


	for(int i = 0; i < nSamples; i++){
	    float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightDir, v3SamplePoint)/fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth * (scale(fLightAngle, fScaleDepth) - scale(fCameraAngle, fScaleDepth)));
		vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColour += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	outData.c0RGB = v3FrontColour * (v3InvWavelength * fKrESun);
	outData.c1RGB = v3FrontColour * fKmESun;
	outData.v3Direction = v3CameraPos - v3Pos;//t0

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

	vec3 v3LightDir;

	vec3 c0RGB;
	vec3 c1RGB;
	vec3 v3Direction;//t0

	vec3 c00RGB;
	vec3 c01RGB;
} outData;

float getNearIntersection(vec3 v3Pos, vec3 v3Ray, float fDistance2, float fRadius2)
{
    float B = 2.0 * dot(v3Pos, v3Ray);
    float C = fDistance2 - fRadius2;
    float fDet = max(0.0, B*B - 4.0 * C);
    return 0.5 * (-B - sqrt(fDet));
}
//Scale
float scale(float fCos, float fScaleDepth)
{
    float x = 1.0 - fCos;
    return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main()
{
    gl_Position = MVP * vec4(position, 1.0);

    outData.position = position;
    outData.normal = normalize(invTModel * vec4(normal, 0.0)).xyz;
    outData.texCoord = texCoord;

	//Atmosphere Edge Code

	float pi = 3.1415926535897932384626433832795f;

	float Kr = 0.0015f;//ray
	float Km = 0.0025f;//mie
	float ESun = 3.0f;
	int nSamples = 4;
	float fSamples = 4.0f;
	vec3 v3CameraPos = vec3(inverse(viewModel) * vec4(0.0f,0.0f,0.0f,1.0f));

	vec3 v3LightDir = vec3(100.0f, 100.0f, 100.0f);
	v3LightDir = normalize(v3LightDir);
	outData.v3LightDir = v3LightDir;
	vec3 v3InvWavelength = vec3(1.0f/pow(0.650f, 4),1.0f/pow(0.570f, 4),1.0f/pow(0.475f, 4));
	float fCameraHeight = sqrt(v3CameraPos.x*v3CameraPos.x+v3CameraPos.y*v3CameraPos.y+v3CameraPos.z*v3CameraPos.z);

	float fCameraHeight2 = fCameraHeight * fCameraHeight;
	float fOuterRadius = 2.050f;
	float fOuterRadius2 = fOuterRadius * fOuterRadius;

	float fInnerRadius = 2.0f;
	float fInnerRadius2 = fInnerRadius * fInnerRadius;
	float fKrESun = Kr * ESun;

	float fKmESun = Km * ESun;
	float fKr4PI = Kr * 4.0f * pi;
	float fKm4PI = Km * 4.0f * pi;

	float fScaleDepth = 0.25f;
	float fInvScaleDepth = 1.0f/fScaleDepth;

	float fScale = 1.0f/(fOuterRadius - fInnerRadius);
	float fScaleOverScaleDepth = fScale/fScaleDepth;

	//mathing
	vec3 v3Pos = position.xyz;
	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	float fNear = getNearIntersection(v3CameraPos, v3Ray, fCameraHeight2, fOuterRadius2);

	vec3 v3Start = v3CameraPos + v3Ray * fNear;//A
	fFar -= fNear;
	float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
	float fStartDepth = exp(-fInvScaleDepth);
	float fStartOffset = fStartDepth * scale(fStartAngle, fScaleDepth);

	float fSampleLength = fFar/fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5f;

	vec3 v3FrontColour = vec3(0.0f, 0.0f, 0.0f);


	for(int i = 0; i < nSamples; i++){
	    float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightDir, v3SamplePoint)/fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth * (scale(fLightAngle, fScaleDepth) - scale(fCameraAngle, fScaleDepth)));
		vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColour += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	outData.c0RGB = v3FrontColour * (v3InvWavelength * fKrESun);
	outData.c1RGB = v3FrontColour * fKmESun;
	outData.v3Direction = v3CameraPos - v3Pos;//t0

>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}