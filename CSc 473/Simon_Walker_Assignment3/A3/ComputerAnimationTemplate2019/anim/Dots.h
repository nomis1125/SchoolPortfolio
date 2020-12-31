#pragma once
#include "BaseSystem.h"
#include <array>
#include <string>

class Dots :
	public BaseSystem
{
public:
	Dots(const std::string& name);

	void Dots::posState(double x, double y, double z);

	void Dots::drawEllipse(float a, float b, float c, float d);

	void Dots::getState(double* p, int i, int j);
	void Dots::setState(double* p, int i, int j);
	void Dots::getStateArray(double* p, int i);
	void Dots::setStateArray(double* p, int i);
	void Dots::getTangentArray(double* p, int i);
	void Dots::setTangentArray(double* p, int i);
	void Dots::setArcLen(std::string a);
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

	std::string textFile;

	//
	Vector hingeFix;
	Vector hingeJoi;
	Vector hingeEnd;
	Vector hingeCt;
	Vector hingeWrist;

	Vector chngPos;

	double* armPt[5];
	//
	Vector m_pos;

	GLMmodel m_model;
};

