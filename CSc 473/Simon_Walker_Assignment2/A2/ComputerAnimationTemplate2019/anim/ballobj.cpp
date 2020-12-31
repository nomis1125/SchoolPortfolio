#include "ballobj.h"

ballobj::ballobj(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	m_mass(1.0f),
	vx(4 * sqrt(2) / 2),
	vy(4 * sqrt(2) / 2),
	numParticles(0)
{

	setVector(m_pos, 0, 0, 0);
	setVector(m_vel, vx, vy, 0);

	for (int i = 0; i < 500; i++) {
		randomx[i] = rand() % 2 -1;
		if (randomx[i] == 0) {
			randomx[i] = 1;
		}
		randomz[i] = rand() % 10;
		setStateArray(m_pos, i);
		setVector(m_vel, vx * randomx[i], vy, randomz[i]);
		setVelArray(m_vel, i);
	}

	setVector(m_pos, 0, 0, 0);
	setVector(m_vel, vx, vy, 1);

}	// ballobj

void ballobj::getState(double* p)
{

	VecCopy(p, m_pos);

}	// ballobj::getState

void ballobj::setState(double* p)
{

	VecCopy(m_pos, p);

}	// ballobj::setState

void ballobj::getStateArray(double* p, int i)
{

	VecCopy(p, m_posArray[i]);

}	// ballobj::getState

void ballobj::setStateArray(double* p, int i)
{

	VecCopy(m_posArray[i], p);

}	// ballobj::setState

void ballobj::setMass(float p, int i)
{

	m_massArray[i] = p;

}	// ballobj::getState

void ballobj::getMass(float p, int i)
{

	p = m_massArray[i];

}	// ballobj::getState

void ballobj::getVel(double* p)
{

	VecCopy(p, m_vel);

}	// ballobj::getState

void ballobj::setVel(double* p)
{

	VecCopy(m_vel, p);

}	// ballobj::setState

void ballobj::getVelArray(double* p, int i)
{

	VecCopy(p, m_velArray[i]);

}	// ballobj::getState

void ballobj::setVelArray(double* p, int i)
{

	VecCopy(m_velArray[i], p);

}	// ballobj::setState

int ballobj::getPartNum() {
	return numParticles;

}

void ballobj::reset(double time)
{
	setVector(m_pos, 0, 0, 0);
	setVector(m_vel, 0, 0, 0);

	for (int i = 0; i < 50; i++) {

		setStateArray(m_pos, i);
		setVelArray(m_vel, i);
	}
	
}	// ballobj::Reset


int ballobj::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "dim") == 0)// Sets the total amount of particles
	{
		if (argc == 2)
		{
			numParticles = atof(argv[1]);

			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: dim <Number of Particles>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "particle") == 0)// Sets a position, mass and velocity for a given particle
	{
		if (argc == 9)
		{
			int i = (int)atof(argv[1]);

			setMass(atof(argv[2]) , i);

			double* holdi = m_pos;
			holdi[0] = atof(argv[3]);
			holdi[1] = atof(argv[4]);
			holdi[2] = atof(argv[5]);
			setStateArray(holdi, i);

			double* holdVeli = m_vel;
			holdVeli[0] = atof(argv[6]);
			holdVeli[1] = atof(argv[7]);
			holdVeli[2] = atof(argv[8]);
			setVelArray(holdVeli, i);

			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: particle <index> <mass> <x y z vx vy vz> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "all_velocities") == 0)// Sets velocity of all particles
	{
		if (argc == 4)
		{
			Vector a;
			setVector(a, 0.0, 0.0, 0.0);
			double* holdVeli = a;

			holdVeli[0] = atof(argv[1]);
			holdVeli[1] = atof(argv[2]);
			holdVeli[2] = atof(argv[3]);

			for (int i = 0; i < numParticles; i++) {
				setVelArray(holdVeli, i);
			}

			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: dim <Number of Particles>");
			return TCL_ERROR;
		}
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
	else if (strcmp(argv[0], "reset") == 0)
	{
		double p[3] = { 0,0,0 };
		setState(p);
	}

	glutPostRedisplay();
	return TCL_OK;

}	// ballobj::command

void ballobj::display(GLenum mode) //start new beginning
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);//keep
	glMatrixMode(GL_MODELVIEW);//keep
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//glTranslated(m_pos[0], m_pos[1], m_pos[2]);//location
	//glScalef(m_sx, m_sy, m_sz);//scale

	//m_mass = m_mass * m_sx * m_sy * m_sz;

	//specify color here
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	for (int i = 0; i < numParticles; i++) {

		Vector m_posi;
		getStateArray(m_posi, i);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(m_posi[0], m_posi[1], m_posi[2]);

	}

	glEnd();

	glPopMatrix();
	glPopAttrib();

}	// ballobj::display
