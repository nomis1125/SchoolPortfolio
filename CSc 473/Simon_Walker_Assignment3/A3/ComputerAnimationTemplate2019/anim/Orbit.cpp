#include "Orbit.h"
#include "Hermite.h"

Orbit::Orbit(const std::string& name, Dots* target) :
	BaseSimulator(name),
	m_object(target),
	oldTime(0.0),
	epsilon(0.2),
	maxSP(100),
	link1(5.0),
	link2(4.0),
	endx(9.0),
	endy(0.0),
	th1(0.0),
	th2(0.0),
	holdTimer(0.0),
	holA(1)
{

	setVector(OgStart, 0.0, 0.0, 0.0);
}	// Orbit

Orbit::~Orbit()
{
}	// Orbit::~Orbit

int Orbit::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0) {

		m_object->setArcLen(argv[1]);
		textFile = argv[1];

		Hermite* hermiteSystem;
		hermiteSystem = new Hermite("hermite");
		const char* c = textFile.c_str();
		hermiteSystem->loadFromFile2D(c);

		VectorObj abc = hermiteSystem->getIntermediatePoint(0.0);
		double* ak = m_vel;
		setVector(ak, abc[0], abc[1], abc[2]);
		m_object->setState(ak, 3, 0);
		m_object->posState(ak[0], ak[1], ak[2]);

		//(OgStart, ak[0], ak[1], ak[2]);
	}
	else
	{
		animTcl::OutputMessage("Working on it!", m_name.c_str());

	}

	glutPostRedisplay();
	return TCL_OK;
}

int Orbit::step(double time)
{

	float posChangeX = 0.0f;
	float posChangeY = 0.0f;

	Vector pos1;
	Vector pos2;
	Vector pos3;
	Vector pos4;
	Vector pos5;

	m_object->getState(pos1, 0, 0);
	m_object->getState(pos2, 1, 0);//upper arm
	m_object->getState(pos3, 2, 0);//lower arm
	m_object->getState(pos4, 3, 0);
	m_object->getState(pos5, 4, 0);//wrist
	float ctptx = pos4[0];
	float ctpty = pos4[1];

	float deltaTime = 0.001;//((float)time - (float)oldTime) / 1000.0;
	oldTime = time;

	float xdir = ctptx - endx;
	float ydir = ctpty - endy;
	float ctdis = sqrt(pow(xdir, 2.0) + pow(ydir, 2.0));

	if (ctdis > epsilon) {
		float xstep = (xdir / ctdis) * maxSP * deltaTime;
		float ystep = (ydir / ctdis) * maxSP * deltaTime;

		endx += xstep;
		endy += ystep;

		float oldTh1 = th1;
		float oldTh2 = th2;

		float thT = atan2(endy, endx);
		th1 = acos((pow(link1, 2.0) + pow(endx, 2.0) + pow(endy, 2.0) - pow(link2, 2.0)) / (2.0 * link1 * sqrt(pow(endx, 2.0) + pow(endy, 2.0)))) + thT;
		th2 = -acos(-1.0 * ((pow(link1, 2.0) + pow(link2, 2.0) - (pow(endx, 2.0) + pow(endy, 2.0))) / (2.0 * link1 * link2)));

		if (isnan(th1)) {
			th1 = oldTh1;
		}
		if (isnan(th2)) {
			th2 = oldTh2;
		}
		
	}
	else {
		holA = 0;
	}

	float p2x = link1 * cos(th1);
	float p2y = link1 * sin(th1);

	float p3x = link1 * cos(th1) + link2 * cos(th1 + th2);
	float p3y = link1 * sin(th1) + link2 * sin(th1 + th2);

	float p4x = p3x;
	float p4y = p3y;

	pos5[0] = p4x + posChangeX;
	pos5[1] = p4y + posChangeY;
	pos5[2] = 0.01;

	pos3[0] = p3x + posChangeX;
	pos3[1] = p3y + posChangeY;
	pos3[2] = 1.0;

	pos2[0] = p2x + posChangeX;
	pos2[1] = p2y + posChangeY;
	pos2[2] = 3.5;

	m_object->setState(pos5, 4, 0);
	m_object->setState(pos3, 2, 0);
	m_object->setState(pos2, 1, 0);

	//
	if (holA == 0) {

		Hermite* hermiteSystem;
		hermiteSystem = new Hermite("hermite");
		const char* c = textFile.c_str();
		hermiteSystem->loadFromFile2D(c);
		VectorObj abc = posHold;
		if ((time-holdTimer) * 0.01 < 1) {
			abc = hermiteSystem->getIntermediatePoint((time-holdTimer) * 0.01);
			posHold = abc;
		}
		m_object->posState(abc[0], abc[1], abc[2]);//get pos from FILE. Look in display 
	}
	else {
		holdTimer = time;
	}
	
	return 0;

}	// Oscillation::step