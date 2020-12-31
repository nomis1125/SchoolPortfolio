#ifndef MYSET
#define MYSET
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

// a sample system
class ballobj : public BaseSystem
{

public:
	ballobj(const std::string& name);
	virtual void getStateArray(double* p, int i);
	virtual void setStateArray(double* p, int i);
	virtual void getState(double* p);
	virtual void setState(double* p);
	virtual void setMass(float p, int i);
	virtual void getMass(float p, int i);
	virtual void getVel(double* p);
	virtual void setVel(double* p);
	virtual void getVelArray(double* p, int i);
	virtual void setVelArray(double* p, int i);
	virtual int getPartNum();
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	float m_mass;

	double vx;
	double vy;

	int numParticles;

	Vector m_pos;
	Vector m_posArray[5000];

	Vector m_vel;
	Vector m_velArray[5000];

	float m_massArray[5000];

	int randomx[5000];
	int randomz[5000];

	GLMmodel m_model;

};
#endif