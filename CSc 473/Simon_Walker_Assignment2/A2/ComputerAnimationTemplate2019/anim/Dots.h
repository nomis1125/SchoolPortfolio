#pragma once
#include "BaseSystem.h"
class Dots :
	public BaseSystem
{
public:
	Dots(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

protected:

	float m_sx;
	float m_sy;
	float m_sz;

	Vector m_pos;

	GLMmodel m_model;
};

