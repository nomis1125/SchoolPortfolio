#ifndef MY_PARTICLE_H
#define MY_PARTICLE_H

/*

	This is a sample system. It accepts the command "read" followed by the 
	path to an OBJ model.

*/


#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"

#include <valarray>
#include <cmath>

struct bodyVec3 {
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	bodyVec3() {}

	bodyVec3(double xx, double yy, double zz) {
		x = xx;
		y = yy;
		z = zz;
	}
};

struct octTree {
	bodyVec3 pos = bodyVec3(0,0,0);
	bodyVec3 vel = bodyVec3(0, 0, 0);
	double mass = 1.0;

	octTree() {}

	octTree(bodyVec3 p, bodyVec3 v, double m) {
		pos = p;
		vel = v;
		mass = m;
	}
};

// a sample system
class SampleParticle : public BaseSystem
{ 

public:
	SampleParticle( const std::string& name );

	int getNum();

	octTree getState(octTree p, int d);
	bodyVec3 getAcc(bodyVec3 p, int d);

	virtual void setState(octTree p, int d);
	virtual void setAcc(bodyVec3 p, int d);
	
	virtual void getBigArray(std::valarray<octTree> p);
	virtual void getBigAcc(std::valarray<bodyVec3> p);
	virtual void setVel(double* p);

	double SampleParticle::getMass();

	void reset( double time );

	void display( GLenum mode = GL_RENDER );

	void readModel(char *fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char **argv) ;

protected:

	float m_sx;
	float m_sy;
	float m_sz;
	float m_mass;

	Vector m_pos ;
	Vector start_pos;
	Vector m_vel;

	GLMmodel m_model ;

	int numbodies;

	std::valarray<bodyVec3> bodyA;//acc
	std::valarray<octTree> octTreeA;//pos, vel, mass

} ;
#endif
