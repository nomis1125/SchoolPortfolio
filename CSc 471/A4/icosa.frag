<<<<<<< HEAD
#version 450 core

const vec3 lightPosition = vec3(0.25, 0.25, 1.0);

const vec3 ca = vec3(0.0, 0.75, 0.75);
const vec3 cd = vec3(0.04, 0.04, 0.04);

in vec3 oUV;
in vec3 tePatchDistance2;
in vec3 normal;

out vec4 fragColour;

vec3 shaded(){
	float df = dot(normal, lightPosition);
	return ca + df * cd;
}

void main(){

	vec3 colour = vec3(0.0);
	colour = shaded();
	float distanceBary = min(min(oUV.x,oUV.y), oUV.z);
	float distanceTriy = min(min(tePatchDistance2.x,tePatchDistance2.y), tePatchDistance2.z);
	
	float o = -0.5;
	float s1 = 40.0;
	float s2 = 60.0;
	
	//**
	distanceTriy = s2 * distanceTriy + o;
	distanceTriy = clamp(distanceTriy, 0, 1);
	distanceTriy = 1 - pow(2, -2 * (distanceTriy * distanceTriy));

	distanceBary = s1 * distanceBary + o;
	distanceBary = clamp(distanceBary, 1, 0);
	//When I clamp via clamp(distanceBary, 0, 1) I only get 1/8th or the sphere. 
	//I am unsure why this happens but doing the above gives me something that 
	//looks like the provided example.
	distanceBary = 1 - pow(2, -2 * (distanceBary * distanceBary));

	colour = distanceBary * distanceTriy * colour;

	fragColour = vec4(colour, 1.0);

=======
#version 450 core

const vec3 lightPosition = vec3(0.25, 0.25, 1.0);

const vec3 ca = vec3(0.0, 0.75, 0.75);
const vec3 cd = vec3(0.04, 0.04, 0.04);

in vec3 oUV;
in vec3 tePatchDistance2;
in vec3 normal;

out vec4 fragColour;

vec3 shaded(){
	float df = dot(normal, lightPosition);
	return ca + df * cd;
}

void main(){

	vec3 colour = vec3(0.0);
	colour = shaded();
	float distanceBary = min(min(oUV.x,oUV.y), oUV.z);
	float distanceTriy = min(min(tePatchDistance2.x,tePatchDistance2.y), tePatchDistance2.z);
	
	float o = -0.5;
	float s1 = 40.0;
	float s2 = 60.0;
	
	//**
	distanceTriy = s2 * distanceTriy + o;
	distanceTriy = clamp(distanceTriy, 0, 1);
	distanceTriy = 1 - pow(2, -2 * (distanceTriy * distanceTriy));

	distanceBary = s1 * distanceBary + o;
	distanceBary = clamp(distanceBary, 1, 0);
	//When I clamp via clamp(distanceBary, 0, 1) I only get 1/8th or the sphere. 
	//I am unsure why this happens but doing the above gives me something that 
	//looks like the provided example.
	distanceBary = 1 - pow(2, -2 * (distanceBary * distanceBary));

	colour = distanceBary * distanceTriy * colour;

	fragColour = vec4(colour, 1.0);

>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}