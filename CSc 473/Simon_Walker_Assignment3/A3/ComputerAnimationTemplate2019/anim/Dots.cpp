#include "Dots.h"
#include <iostream>
#include <fstream>
#include <string>
#include "hermite.h"
#include <string>
using namespace std;

Dots::Dots(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	currPoint(0)
{

	setVector(m_pos, 0, 0, 0);
	curveType = 0.0f;
	for (int i = 0; i < 40; i++) {
		ArcLen[i] = 0.0;
	}
	textFile = "null";
	
	setVector(chngPos, 0, 0, 0);//pos of bob

	//

	setVector(hingeCt, -2, -1.5, 0);//9.5, -1.5, 5);//heading to this point
	
	setVector(hingeFix, -1, -3, 5);//0.0, -1.5, 5);//attatchBody
	setVector(hingeJoi, -9, -3, 5);//5, -1.5, 5);//upperarm Link2
	setVector(hingeEnd, -4, -3, 5);//10, -1.5, 5);//lowerarm* Link1
	setVector(hingeWrist, -10, -3, 5);

	armPt[0] = hingeFix;
	armPt[1] = hingeJoi;
	armPt[2] = hingeEnd;//**
	armPt[3] = hingeCt;
	armPt[4] = hingeWrist;

}	// SampleParticle

void Dots::posState(double x, double y, double z)
{
	double* hold = hingeCt;
	hold[0] = x;
	hold[1] = y;
	hold[2] = z;

	setState(hold, 3, 0);

}

void Dots::drawEllipse(float a, float b, float c, float d) {// a = x radiud; b = x position; c = y radi; d = y position

	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);

	for (int i = 0; i < 360; i++) {
		float rad = i * 3.14159/180.0;

		glVertex3f(cos(rad) * a + b + chngPos[0], sin(rad) * c + d + chngPos[1], 5+ chngPos[2]);
	}

	glEnd();
}

void Dots::getState(double* p, int i, int j)
{
	double* am = armPt[i];
	VecCopy(p, am);

}	// SampleParticle::getState

void Dots::setState(double* p, int i, int j)
{
	double* am = armPt[i];
	VecCopy(am, p);

}	// SampleParticle::setState

void Dots::getStateArray(double* p, int i)
{

	VecCopy(p, holdPoints[i]);

}	// SampleParticle::getState

void Dots::setStateArray(double* p, int i)
{

	VecCopy(holdPoints[i], p);

}	// SampleParticle::setState
void Dots::getTangentArray(double* p, int i)
{

	VecCopy(p, holdTangent[i]);

}	// SampleParticle::getState

void Dots::setTangentArray(double* p, int i)
{

	VecCopy(holdTangent[i], p);

}	// SampleParticle::setState

void Dots::setArcLen(std::string a) {
	textFile = a;
}

void Dots::reset(double time)
{

	setVector(m_pos, 0, 0, 0);
	int currPoint = 0;
	Vector a;
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	for (int i = 0; i < 40; i++) {
		setStateArray(a, i);
		setTangentArray(a, i);
		ArcLen[i] = 0.0;
	}

}	// SampleParticle::Reset


int Dots::command(int argc, myCONST_SPEC char** argv)
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
	else if (strcmp(argv[0], "position") == 0)
	{
		if (argc == 4)
		{
			chngPos[0] = atof(argv[1]);
			chngPos[1] = atof(argv[2]);
			chngPos[2] = atof(argv[3]);

		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "cr") == 0)
	{
		animTcl::OutputMessage("Catmull-Rom initializing.", m_name.c_str());

		if (argc == 1) {
			curveType = 1.0f;
			Dots::display();
		}
		else {
			animTcl::OutputMessage("Usage: <name> cr ");
			return TCL_ERROR;
		}
	}//Catmull-Rom Equation
	else if (strcmp(argv[0], "hs") == 0)
	{
		animTcl::OutputMessage("Hermit Spline initializing.", m_name.c_str());

		if (argc == 1) {
			curveType = 0.0f;
			Dots::display();
		}
		else {
			animTcl::OutputMessage("Usage: <name> hs ");
			return TCL_ERROR;
		}
	}//hermit spline equation
	else if (strcmp(argv[0], "loadFile") == 0)
	{
		//animTcl::OutputMessage("loadFile ON.");// , m_name.c_str());

		if (argc == 2) {
			double length = 0.0;
			for (int i = 0; i < currPoint-1; i++) {
				length += ArcLen[i];
			}
			
			double mk = atoi(argv[1]);
			length = length *mk;
			animTcl::OutputMessage("%f", length);
		}
		else {
			animTcl::OutputMessage("Usage:  > system <name> getArcLength <t>");
			return TCL_ERROR;
		}
	}//hermit spline equation
	else if (strcmp(argv[0], "set") == 0 && strcmp(argv[1], "tangent") == 0) {
		if (argc == 6) {
			Vector a;
			int pos = atof(argv[2]);
			a[0] = atof(argv[3]);
			a[1] = atof(argv[4]);
			a[2] = atof(argv[5]);

			setVector(holdTangent[pos], a[0], a[1], a[2]);
			Vector b;
			setVector(b, 0, 0, 0);

			VecCopy(b, holdTangent[pos]);

			if (b[0] == a[0] && b[1] == a[1] && b[2] == a[2]) {
				//animTcl::OutputMessage("Success");
			}
			else {
				//animTcl::OutputMessage("Fail");
			}
			
		}
		else {
			animTcl::OutputMessage("Usage: <name> set tangent <index> <x y z> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "set") == 0 && strcmp(argv[1], "point") == 0) {
		if (argc == 6) {
			Vector a;
			int pos = atof(argv[2]);
			a[0] = atof(argv[3]);
			a[1] = atof(argv[4]);
			a[2] = atof(argv[5]);

			setVector(holdPoints[pos], a[0], a[1], a[2]);
			Vector b;
			setVector(b, 0, 0, 0);

			VecCopy(b, holdPoints[pos]);

			if (b[0] == a[0] && b[1] == a[1] && b[2] == a[2]) {
				//animTcl::OutputMessage("Success");
			}
			else {
				//animTcl::OutputMessage("Fail");
			}

		}
		else {
			animTcl::OutputMessage("Usage: <name> set tangent <index> <x y z> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "add") == 0 && strcmp(argv[1], "point") == 0) {
		if (argc == 8) {
			Vector a;
			Vector b;
			a[0] = atof(argv[2]);
			a[1] = atof(argv[3]);
			a[2] = atof(argv[4]);

			b[0] = atof(argv[5]);
			b[1] = atof(argv[6]);
			b[2] = atof(argv[7]);

			setVector(holdPoints[currPoint], a[0], a[1], a[2]);//point
			setVector(holdTangent[currPoint], b[0], b[1], b[2]);//tan
			Vector c;
			setVector(c, 0, 0, 0);

			VecCopy(c, holdPoints[currPoint]);

			if (c[0] == a[0] && c[1] == a[1] && c[2] == a[2]) {
				//animTcl::OutputMessage("Success");
			}
			else {
				//animTcl::OutputMessage("Fail");
			}
			currPoint++;
		}
		else {
			animTcl::OutputMessage("Usage: <name> set tangent <index> <x y z> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "load") == 0)
	{
		if (argc == 2) {

		}
		else {
			animTcl::OutputMessage("Usage: system <name> load \"< file name >\"");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "export") == 0)
	{
		if (argc == 2) {
			ofstream myFile (argv[1]);

			if (myFile.is_open()) {
				if (curveType == 0.0f) {
					myFile << "hermite ";
					myFile << currPoint;
					myFile << "\n";
				}
				else {
					myFile << "catmull-rom";
					myFile << currPoint;
					myFile << "\n";
				}
				for (int i = 0; i < currPoint; i++) {
					Vector poi;
					Vector tan;
					getStateArray(poi, i);
					getTangentArray(tan, i);

					myFile << poi[0];
					myFile << " ";
					myFile << poi[1];
					myFile << " ";
					myFile << poi[2];
					myFile << " ";

					myFile << tan[0];
					myFile << " ";
					myFile << tan[1];
					myFile << " ";
					myFile << tan[2];
					myFile << "\n";
				}
				myFile.close();
			}
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
	
	//Wall
	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(-20, 20, -0.02);
	glVertex3f(20, 20, -0.02);
	glVertex3f(20, -20, -0.02);
	glVertex3f(-20, -20, -0.02);
	glEnd();
	
	//Floor
	glBegin(GL_POLYGON);
	glColor3f(0.58, 0.29, 0);
	glVertex3f(-20, -20, 40);
	glVertex3f(20, -20, 40);
	glVertex3f(20, -20, 0);
	glVertex3f(-20, -20, 0);
	glEnd();

	//BlackBoard
	glBegin(GL_POLYGON);
	glColor3f(0, 0.3, 0.01);
	glVertex3f(-14, 12, -0.01);
	glVertex3f(14, 12, -0.01);
	glVertex3f(14, -12, -0.01);
	glVertex3f(-14, -12, -0.01);
	glEnd();

	//hermite display
	if (textFile.compare("null") != 0)
	{
		Hermite* hermiteSystem;
		hermiteSystem = new Hermite("hermite");
		const char* c = textFile.c_str();
		hermiteSystem->loadFromFile2D(c);
		hermiteSystem->display();
	}

	//person | a = x radius; b = x position; c = y radi; d = y position
	//a b c d
	drawEllipse(2, 0, 2, 0);//head;
	drawEllipse(2, 0, 4, -6);//body; 

	drawEllipse(2, -3, 0.5, -3);//left bicep
	drawEllipse(2, -7, 0.5, -3);//left forearm
	drawEllipse(1, -10, 0.25, -3);//left hand

	drawEllipse(2, 3, 0.5, -3);//right bicep
	drawEllipse(2, 7, 0.5, -3);//right forearm
	drawEllipse(1, 10, 0.25, -3);//right hand

	drawEllipse(0.75, -1, 2.5, -13);//left thigh
	drawEllipse(0.75, -1, 2.5, -17);//left shin
	drawEllipse(1, -1, 0.5, -19.5);//left foot

	drawEllipse(0.75, 1, 2.5, -13);//right thigh
	drawEllipse(0.75, 1, 2.5, -17);//right shin
	drawEllipse(1, 1, 0.5, -19.5);//right foot

	//
	double* hold = armPt[0];
	//Drawing the Joints:
	/*
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	hold = armPt[0];//A
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(hold[0], hold[1], hold[2]);

	hold = armPt[1];//B
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);//TL P0

	hold = armPt[2];//C
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);//TL P0

	hold = armPt[4];//D
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);

	glEnd();
	*/

	//Line A to B
	glBegin(GL_LINES);
	hold = armPt[0];
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);
	hold = armPt[1];
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);//TL P0
	glEnd();

	//Line B to C
	glBegin(GL_LINES);

	hold = armPt[1];
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);//TL P0

	hold = armPt[2];
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);//TL P0

	//Line C to D
	glBegin(GL_LINES);

	hold = armPt[2];
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);//TL P0

	hold = armPt[4];
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(hold[0], hold[1], hold[2]);//TL P0
	
	glEnd();

	glPopMatrix();
	glPopAttrib();
}	// SampleParticle::display