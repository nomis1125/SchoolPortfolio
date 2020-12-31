#include "Plane.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Plane::Plane(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{

	setVector(m_pos, 0, 0, 0);
	setVector(start_pos, 0, -5, 0);
	setVector(m_pos, start_pos[0], start_pos[1], start_pos[2]);
}	// SampleParticle

void Plane::getState(double* p)
{

	VecCopy(p, m_pos);

}	// SampleParticle::getState

void Plane::setState(double* p)
{

	VecCopy(m_pos, p);

}	// SampleParticle::setState

void Plane::reset(double time)
{

	setVector(m_pos, start_pos[0], start_pos[1], start_pos[2]);

}	// SampleParticle::Reset


int Plane::command(int argc, myCONST_SPEC char** argv)
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
	else if (strcmp(argv[0], "pos") == 0)
	{
		if (argc == 4)
		{
			m_pos[0] = atof(argv[1]);
			m_pos[1] = atof(argv[2]);
			m_pos[2] = atof(argv[3]);

			start_pos[0] = atof(argv[1]);
			start_pos[1] = atof(argv[2]);
			start_pos[2] = atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else
	{
		animTcl::OutputMessage("Working on it!", m_name.c_str());

	}

	glutPostRedisplay();
	return TCL_OK;

}	// SampleParticle::command

void Plane::display(GLenum mode) //start new beginning
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);//keep
	glMatrixMode(GL_MODELVIEW);//keep
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//specify color here
	//glTranslated(m_pos[0], m_pos[1], m_pos[2]);//location
	glScalef(m_sx, m_sy, m_sz);//scale

	glBegin(GL_LINE_STRIP);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(10, -5, 0);
	start_pos[0] = 10;
	start_pos[1] = -5;
	start_pos[2] = 0;
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-10, -5, 0);

	glEnd();


	glPopMatrix();
	glPopAttrib();
}	// SampleParticle::display