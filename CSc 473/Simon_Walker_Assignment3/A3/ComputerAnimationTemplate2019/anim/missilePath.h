#pragma once
#include "BaseSystem.h"

#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"
class missilePath :
	public BaseSystem
{


public:
	missilePath(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);

	void missilePath::getHoldPoints2Len(int p);
	void missilePath::getCurrPos(int p);

	void missilePath::getStateArray(double* p, int i);
	void missilePath::setStateArray(double* p, int i);

	void missilePath::getStateArray2(double* p);
	void missilePath::setStateArray2(double* p, int i);
	void missilePath::nextStateArray2();

	void missilePath::getTangentArray(double* p, int i);
	void missilePath::setTangentArray(double* p, int i);

	void missilePath::positioning(double* a, double* b, double* c, double* d);

	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	float curveType;

	double speed;

	Vector holdPoints2[1600];

	Vector holdPoints[40];
	Vector holdTangent[40];
	double quarternion[4];
	double holdpos[3];
	double ArcLen[40];

	int currPoint;
	int nowpos;
	int chkpt;

	Vector m_pos;

	GLMmodel m_model;

};