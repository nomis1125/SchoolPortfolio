#include "Dots.h"

Dots::Dots(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{

	setVector(m_pos, 0, 0, 0);

}	// SampleParticle

void Dots::getState(double* p)
{

	VecCopy(p, m_pos);

}	// SampleParticle::getState

void Dots::setState(double* p)
{

	VecCopy(m_pos, p);

}	// SampleParticle::setState

void Dots::reset(double time)
{

	setVector(m_pos, 0, 0, 0);

}	// SampleParticle::Reset


int Dots::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
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

void Dots::display(GLenum mode) //start new beginning
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);//keep
	glMatrixMode(GL_MODELVIEW);//keep
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//specify color here
	glTranslated(m_pos[0], m_pos[1], m_pos[2]);//location
	glScalef(m_sx, m_sy, m_sz);//scale

	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	double P0x = -3.0f; double P0y = -3.0f; double P0z = 0.0f;
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(P0x, P0y, P0z);//TL P0
	double P1x = -3.0f; double P1y = 2.0f; double P1z = 0.0f;
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(P1x, P1y, P1z);//TR P1
	double P2x = 1.0f; double P2y = 5.5f; double P2z = 0.0f;
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(P2x, P2y, P2z);//BR P2
	double P3x = 5.0f; double P3y = 0.0f; double P3z = 0.0f;
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(P3x, P3y, P3z);//BL P3

	glEnd();

	
	//
	glBegin(GL_LINE_STRIP);
	double x, y, z;
	for (float t = 0.0f; t < 0.98f;) {
		glColor3f(0.0f, 0.0f, 0.0f);

		x = pow((1 - t), 3) * (P0x) + 3 * t * pow((1 - t), 2) * (P1x) + 3 * pow(t, 2) * (1 - t) * (P2x) + pow(t, 3) * (P3x);
		y = pow((1 - t), 3) * (P0y) + 3 * t * pow((1 - t), 2) * (P1y) + 3 * pow(t, 2) * (1 - t) * (P2y) + pow(t, 3) * (P3y);
		z = pow((1 - t), 3) * (P0z) + 3 * t * pow((1 - t), 2) * (P1z) + 3 * pow(t, 2) * (1 - t) * (P2z) + pow(t, 3) * (P3z);

		glVertex3f(x, y, z);

		t += 0.02f;

		x = pow((1 - t), 3) * (P0x)+3 * t * pow((1 - t), 2) * (P1x)+3 * pow(t, 2) * (1 - t) * (P2x)+pow(t, 3) * (P3x);
		y = pow((1 - t), 3) * (P0y)+3 * t * pow((1 - t), 2) * (P1y)+3 * pow(t, 2) * (1 - t) * (P2y)+pow(t, 3) * (P3y);
		z = pow((1 - t), 3) * (P0z)+3 * t * pow((1 - t), 2) * (P1z)+3 * pow(t, 2) * (1 - t) * (P2z)+pow(t, 3) * (P3z);

		glVertex3f(x, y, z);
	}
	glEnd();

	glPopMatrix();
	glPopAttrib();
}	// SampleParticle::display

/*
glColor3f(1.0f, 0.0f, 0.0f);
	double P0x = -2.0f; double P0y = 2.0f; double P0z = 0.0f;
	glVertex3f(-2.0f, 2.0f, 0.0f);//TL P0
	double P1x = 2.0f; double P1y = 2.0f; double P1z = 0.0f;
	glVertex3f(2.0f, 2.0f, 0.0f);//TR P1
	double P2x = 2.0f; double P2y = -2.0f; double P2z = 0.0f;
	glVertex3f(2.0f, -2.0f, 0.0f);//BR P2
	double P3x = -2.0f; double P3y = -2.0f; double P3z = 0.0f;
	glVertex3f(-2.0f, -2.0f, 0.0f);//BL P3
*/