#pragma once
#include "BaseSystem.h"

#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"
class tankPath :
	public BaseSystem
{


public:
	tankPath(const std::string& name);

	void tankPath::getSpeed(double t);

	virtual void getState(double* p);
	virtual void setState(double* p);

	void tankPath::getHoldPoints2Len(int p);
	void tankPath::getCurrPos(int p);

	void tankPath::getStateArray(double* p, int i);
	void tankPath::setStateArray(double* p, int i);

	void tankPath::getStateArray2(double* p);
	void tankPath::setStateArray2(double* p, int i);
	void tankPath::nextStateArray2();

	void tankPath::setTangentArray2(double* p, int i);
	void tankPath::getTangentArray2(double* p);

	void tankPath::nextbigCurrPt();

	void tankPath::getTangentArray(double* p, int i);
	void tankPath::setTangentArray(double* p, int i);

	void tankPath::hermiteSpline();

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

	Vector holdPoints2[16000];
	Vector holdTangents2[16000];

	Vector holdPoints[40];
	Vector holdTangent[40];
	//Vector points[];
	double ArcLen[40];

	int currPoint;
	int nowpos;
	int chkpt;

	int bigCurrPt;

	Vector m_pos;

	GLMmodel m_model;

};