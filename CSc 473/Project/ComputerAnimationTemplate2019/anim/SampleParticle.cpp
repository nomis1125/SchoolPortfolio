#include "SampleParticle.h"

SampleParticle::SampleParticle(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	m_mass(10.0f)
{ 

	setVector( m_pos, 0, 0, 0 );
	setVector(start_pos, 0, 0, 0);
	setVector(m_vel, 0, 0, 0);

	numbodies = 5000;//500 good number
	
	bodyA.resize(numbodies);
	octTreeA.resize(numbodies);

	bodyA[0] = bodyVec3(0,0,0);
	octTreeA[0].pos = bodyVec3(0, 0, 0);
	octTreeA[0].vel = bodyVec3(0, 0, 0);
	octTreeA[0].mass = 10.0;

	double mk = 0;

	for (int i = 1; i < numbodies; i++) {
		octTreeA[i].pos.x = (rand() % 100) - 50;//cos(i) + mk;
		octTreeA[i].pos.y = (rand() % 100) - 50;//sin(i) + mk;
		octTreeA[i].pos.z = (rand() % 100) - 50;//(sin(i)/cos(i)) + mk;

		octTreeA[i].vel.x = 0.0;
		octTreeA[i].vel.y = 0.0;
		octTreeA[i].vel.z = 0.0;

		if (i > 10000) {
			octTreeA[i].mass = 10.0;
		}
		else {
			octTreeA[i].mass = (rand() % 100) + 1;
		}

		bodyA[i].x = 0.0;
		bodyA[i].y = 0.0;
		bodyA[i].z = 0.0;
	}

}	// SampleParticle

int SampleParticle::getNum() {
	return numbodies;
}

octTree SampleParticle::getState( octTree p, int d)
{ 

	return octTreeA[d];

}	// SampleParticle::getState

bodyVec3 SampleParticle::getAcc( bodyVec3 p, int d )
{ 

	return bodyA[d];

}	// SampleParticle::setState
void SampleParticle::setState(octTree p, int d)
{

	octTreeA[d] = p;

}	// SampleParticle::getState

void SampleParticle::setAcc(bodyVec3 p, int d)
{

	bodyA[d] = p;

}	// SampleParticle::setState


void SampleParticle::getBigArray(std::valarray<octTree> d)
{
	for (int i = 0; i < numbodies; i++) {
		d[i] = octTreeA[i];
	}

}	// SampleParticle::getState

void SampleParticle::getBigAcc(std::valarray<bodyVec3> d)
{
	for (int i = 0; i < numbodies; i++) {
		d[i] = bodyA[i];
	}

}	// SampleParticle::getState

void SampleParticle::setVel(double* p)
{

	VecCopy(m_vel, p);

}	// SampleParticle::setState

double SampleParticle::getMass()
{

	return m_mass;

}	// SampleParticle::getState

void SampleParticle::reset( double time ) 
{ 

	setVector( m_pos, start_pos[0], start_pos[1], start_pos[2]);
	setVector(m_vel, 0, 0, 0);
	
}	// SampleParticle::Reset


int SampleParticle::command(int argc, myCONST_SPEC char **argv) 
{
	if( argc < 1 )
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str()) ;
		return TCL_ERROR ;
	}
	else if( strcmp(argv[0], "read") == 0 )
	{
		if( argc == 2 )
		{
			m_model.ReadOBJ(argv[1]) ;
			glmFacetNormals(&m_model) ;
			glmVertexNormals(&m_model, 90) ;
			return TCL_OK ;
		}
		else 
		{
			animTcl::OutputMessage("Usage: read <file_name>") ;
			return TCL_ERROR ;
		}
	}
	else if( strcmp(argv[0], "scale") == 0 )
	{
		if( argc == 4 )
		{
			m_sx = (float)atof(argv[1]) ;
			m_sy = (float)atof(argv[2]) ;
			m_sz = (float)atof(argv[3]) ;
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ") ;
			return TCL_ERROR ;

		}
	}
	else if( strcmp(argv[0], "pos") == 0 )
	{
		if( argc == 4 )
		{
			m_pos[0] = atof(argv[1]) ;
			m_pos[1] = atof(argv[2]) ;
			m_pos[2] = atof(argv[3]) ;

			start_pos[0] = atof(argv[1]);
			start_pos[1] = atof(argv[2]);
			start_pos[2] = atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ") ;
			return TCL_ERROR ;

		}
	}
	else if( strcmp(argv[0], "flipNormals") == 0 )
	{
		flipNormals() ;
		return TCL_OK ;
		
	}
	else if( strcmp(argv[0], "reset") == 0)
	{
		//double p[3] = { start_pos[0],start_pos[1],start_pos[2]} ;
		for (int i = 0; i < numbodies; i++) {
			octTreeA[i].pos.x = i;
			octTreeA[i].pos.y = 0.0;
			octTreeA[i].pos.z = 0.0;
			octTreeA[i].vel.x = 0.0;
			octTreeA[i].vel.y = sqrt(1.0 / i);
			octTreeA[i].vel.z = 0.0;
		}
	}
    
    glutPostRedisplay() ;
	return TCL_OK ;

}	// SampleParticle::command

void SampleParticle::display( GLenum mode ) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_MODELVIEW) ;
	glPushMatrix() ;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(m_pos[0],m_pos[1],m_pos[2]) ;
	glScalef(m_sx,m_sy,m_sz) ;

	glEnable(GL_POINT_SMOOTH);
	glPointSize(1.0f);

	glBegin(GL_POINTS);

	for (int i = 0; i < numbodies; i++) {
		
		glColor3f(1.0f, 1.0f, 0.3f);
		//glColor3f(0.1 * octTreeA[i].mass, 1.0f, 0.3 * octTreeA[i].mass);
		glVertex3f(octTreeA[i].pos.x, octTreeA[i].pos.y, octTreeA[i].pos.z);
	}

	glEnd();

	glPopMatrix();
	glPopAttrib();

}	// SampleParticle::display
