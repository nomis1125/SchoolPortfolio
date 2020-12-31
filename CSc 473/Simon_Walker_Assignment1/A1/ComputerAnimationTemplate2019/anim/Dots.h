#pragma once
#include "BaseSystem.h"
#include <array>
class Dots :
	public BaseSystem
{
public:
	Dots(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void Dots::getStateArray(double* p, int i);
	void Dots::setStateArray(double* p, int i);
	void Dots::getTangentArray(double* p, int i);
	void Dots::setTangentArray(double* p, int i);
	void Dots::setArcLen(double p, int i);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	double curveType;//Set for Hermite Spline/Carmull-rom

	Vector holdPoints[40];
	Vector holdTangent[40];
	//Vector points[];
	double ArcLen[40];

	int currPoint;

	Vector m_pos;

	GLMmodel m_model;
};

