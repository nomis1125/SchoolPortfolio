#include "missilePath.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

missilePath::missilePath(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	nowpos(0),
	chkpt(0),
	speed(20)
{

	setVector(m_pos, holdpos[0], holdpos[1], holdpos[2]);
	curveType = 0.0f;
}	// missilePath

void missilePath::getState(double* p)
{

	VecCopy(p, m_pos);

}	// missilePath::getState

void missilePath::setState(double* p)
{

	VecCopy(m_pos, p);

}	// missilePath::setState

void missilePath::getHoldPoints2Len(int p)
{

	p = chkpt;

}	// missilePath::setState

void missilePath::getCurrPos(int p)
{

	p = nowpos;

}	// missilePath::setState

void missilePath::getStateArray(double* p, int i)
{

	VecCopy(p, holdPoints[i]);

}	// SampleParticle::getState

void missilePath::setStateArray(double* p, int i)
{

	VecCopy(holdPoints[i], p);

}	// SampleParticle::setState

void missilePath::getStateArray2(double* p)
{

	VecCopy(p, holdPoints2[nowpos]);

}	// SampleParticle::getState

void missilePath::setStateArray2(double* p, int i)
{

	VecCopy(holdPoints2[i], p);

}	// SampleParticle::setState
void missilePath::nextStateArray2()
{
	nowpos++;

}	// SampleParticle::setState

void missilePath::getTangentArray(double* p, int i)
{

	VecCopy(p, holdTangent[i]);

}	// SampleParticle::getState

void missilePath::setTangentArray(double* p, int i)
{

	VecCopy(holdTangent[i], p);

}	// SampleParticle::setState

void missilePath::positioning(double* a, double* b, double* c, double* d)//missile, tan, tank pos, return
{
	

	double x, y, z;

	//for (double t = 0.0f; t < 1.0f; t += 0.0025f) {
	double t = 0.025f * speed;
		//P1 || f1(t)
		x = ((2.0 * pow(t, 3)) - (3.0 * pow(t, 2)) + 1.0) * a[0];
		y = ((2.0 * pow(t, 3)) - (3.0 * pow(t, 2)) + 1.0) * a[1];
		z = ((2.0 * pow(t, 3)) - (3.0 * pow(t, 2)) + 1.0) * a[2];

		//P1' || f3(t)
		x = (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[0];
		y = (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[1];
		z = (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[2];

		//t += 0.025f;

		//P2 || f2(t)
		x += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[0];
		y += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[1];
		z += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[2];

		//P2' || f4(t)
		x += (pow(t, 3) - pow(t, 2)) * b[0];
		y += (pow(t, 3) - pow(t, 2)) * b[1];
		z += (pow(t, 3) - pow(t, 2)) * b[2];

		d[0] = x;
		d[1] = y;
		d[2] = z;
	//}
}

void missilePath::reset(double time)
{
	setVector(m_pos, holdpos[0], holdpos[1], holdpos[2]);
	currPoint = 0;
	nowpos = 0;
	speed = 1;
	Vector a;
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	for (int i = 0; i < 40; i++) {
		setStateArray(a, i);
		setTangentArray(a, i);
		ArcLen[i] = 0.0;
	}

}	// missilePath::Reset


int missilePath::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0)
	{
		if (argc == 2)
		{
			m_model.ReadOBJ(argv[1]);
			glmFacetNormals(&m_model);
			glmVertexNormals(&m_model, 90);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: read <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{
			m_sx = (float)atof(argv[1]);
			m_sy = (float)atof(argv[2]);
			m_sz = (float)atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "speed") == 0)
	{
		if (argc == 2)
		{
			speed = atof(argv[1]);
		}
		else
		{
			animTcl::OutputMessage("Usage: speed <num> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "pos") == 0)
	{
		if (argc == 4)
		{
			m_pos[0] = atof(argv[1]);
			m_pos[1] = atof(argv[2]);
			m_pos[2] = atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "flipNormals") == 0)
	{
		flipNormals();
		return TCL_OK;

	}
	else if (strcmp(argv[0], "state") == 0) {
		if (argc == 8)
		{
			m_pos[0] = atof(argv[1]);
			m_pos[1] = atof(argv[2]);
			m_pos[2] = atof(argv[3]);

			holdpos[0] = m_pos[0];
			holdpos[1] = m_pos[1];
			holdpos[2] = m_pos[2];

			quarternion[0] = atof(argv[4]);
			quarternion[1] = atof(argv[5]);
			quarternion[2] = atof(argv[6]);
			quarternion[3] = atof(argv[7]);
		}
		else
		{
			animTcl::OutputMessage("Usage: <x y z v1 v2 v3 e>  ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "reset") == 0)
	{
		double p[3] = { 0,0,0 };
		setState(p);
	}

	glutPostRedisplay();
	return TCL_OK;

}	// missilePath::command

void missilePath::display(GLenum mode) //start new beginning
{
	glEnable(GL_LIGHTING);//keep
	glMatrixMode(GL_MODELVIEW);//keep
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//specify color here
	glTranslated(m_pos[0], m_pos[1], m_pos[2]);//location
	glScalef(m_sx, m_sy, m_sz);//scale

	//glcolor

	if (m_model.numvertices > 0)
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0, 20, 20); //sphere being drawn

	glPopMatrix();
	glPopAttrib();

}	// missilePath::display
