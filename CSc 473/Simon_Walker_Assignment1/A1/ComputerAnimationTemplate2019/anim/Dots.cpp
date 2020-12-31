#include "Dots.h"
#include <iostream>
#include <fstream>
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
}	// SampleParticle

void Dots::getState(double* p)
{

	VecCopy(p, m_pos);

}	// SampleParticle::getState

void Dots::setState(double* p)
{

	VecCopy(m_pos, p);

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

void Dots::setArcLen(double p, int i) {
	ArcLen[i] = p;
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
	else if (strcmp(argv[0], "getArcLength") == 0)
	{
		animTcl::OutputMessage("Getting Arc Length. Note: Won't work unless manually put in.");// , m_name.c_str());

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
			ifstream inFile;
			inFile.open(argv[1]);

			if (!inFile) {
				animTcl::OutputMessage("File not found.");
				inFile.close();
				return TCL_ERROR;
			}
			string line;
			int location = -1;
			while (getline(inFile, line)) {
				if (location == -1) {
					string cmps = line.substr(0, 8);
					if (cmps.compare("hermite")) {
						curveType = 0.0f;
					}
					else {
						animTcl::OutputMessage("Hermite not found.");
						inFile.close();
						return TCL_ERROR;
					}
					string cmpnm = line.substr(line.find(" "));
					int num = stoi(cmpnm);
					currPoint = num;
					location++;
				}
				else {
					Vector a;
					Vector b;
					int posi = 0;
					string prev = line.substr(posi, line.find(' '));
					line = line.substr(prev.length()+1);
					a[0] = stod(prev);

					prev = line.substr(posi, line.find(' '));
					line = line.substr(prev.length() + 1);
					a[1] = stod(prev);

					prev = line.substr(posi, line.find(' '));
					line = line.substr(prev.length() + 1);
					a[2] = stod(prev);

					prev = line.substr(posi, line.find(' '));
					line = line.substr(prev.length() + 1);
					b[0] = stod(prev);

					prev = line.substr(posi, line.find(' '));
					line = line.substr(prev.length() + 1);
					b[1] = stod(prev);

					prev = line.substr(posi, line.find('\n'));
					line = line.substr(prev.length());
					b[2] = stod(prev);

					setVector(holdPoints[location], a[0], a[1], a[2]);//point
					setVector(holdTangent[location], b[0], b[1], b[2]);//tan
					location++;
				}
			}
			inFile.close();
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

	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0f);
	
	/*glBegin(GL_POINTS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);//default centre
	glEnd();*/

	if (currPoint >= 4) {
		int nextPoint = currPoint;
		int chkpt = 0;//made for firs run of catmull-rom
		for (int i = 0; i < nextPoint; i++) {
			Vector a;
			Vector b;
			Vector c;
			Vector d;

			//for ArcLen
			double vecX;
			double vecY;
			double vecZ;

			double holdArcLen = 0.0;

			//Hermite Spline
			if (curveType == 0.0f) {

				getStateArray(a, i);//point A
				getTangentArray(b, i);
				getStateArray(c, i+1);//point B
				getTangentArray(d, i+1);

				glBegin(GL_POINTS);

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(a[0], a[1], a[2]);

				glEnd();

				glBegin(GL_LINE_STRIP);
				double x, y, z;
				double oldx = 0.0;
				double oldy = 0.0;
				double oldz = 0.0;

				for (double t = 0.0f; t < 1.0f; t += 0.0025f) {
					glColor3f(0.0f, 0.0f, 0.0f);

					//P1 || f1(t)
					x = ((2.0 * pow(t, 3)) - (3.0 * pow(t, 2)) + 1.0) * a[0];
					y = ((2.0 * pow(t, 3)) - (3.0 * pow(t, 2)) + 1.0) * a[1];
					z = ((2.0 * pow(t, 3)) - (3.0 * pow(t, 2)) + 1.0) * a[2];

					//P1' || f3(t)
					x += (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[0];
					y += (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[1];
					z += (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[2];

					//t += 0.025f;

					//P2 || f2(t)
					x += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[0];
					y += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[1];
					z += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[2];

					//P2' || f4(t)
					x += (pow(t, 3) - pow(t, 2)) * d[0];
					y += (pow(t, 3) - pow(t, 2)) * d[1];
					z += (pow(t, 3) - pow(t, 2)) * d[2];

					glVertex3f(x, y, z);

					//length between points
					vecX = oldx - x;
					vecY = oldy - y;
					vecZ = oldz - z;

					vecX = pow(vecX, 2);
					vecY = pow(vecY, 2);
					vecZ = pow(vecZ, 2);

					holdArcLen += sqrt(vecX + vecY + vecZ);

					//animTcl::OutputMessage("%d", holdArcLen);

					oldx = x;
					oldy = y;
					oldz = z;
				}

				//arcLen
				ArcLen[i] = holdArcLen/40.0;

				glEnd();

			}
			else if (curveType == 1.0f) {//catmull-rom
				nextPoint = currPoint - 3;
				if (chkpt == 0) {
					getTangentArray(a, i);
					getStateArray(b, i);
					getStateArray(c, i + 1);
					getStateArray(d, i + 2);
					i = -1;
					chkpt++;
				}
				else if (chkpt == -1) {
					getStateArray(a, i);
					getStateArray(b, i + 1);
					getStateArray(c, i + 2);
					getTangentArray(d, i + 2);
					chkpt = -2;
				}
				else {
					getStateArray(a, i);
					getStateArray(b, i + 1);
					getStateArray(c, i + 2);
					getStateArray(d, i + 3);
				}

				/*glBegin(GL_POINTS);

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(a[0], a[1], a[2]);

				glEnd();*/

				glBegin(GL_LINE_STRIP);
				double x, y, z, s;//s is the tension parameter
				double oldx = 0.0;
				double oldy = 0.0;
				double oldz = 0.0;

				s = 0.5;
				for (double t = 0.0; t < 1.0; t += 0.0025) {
					glColor3f(0.0f, 0.0f, 0.0f);

					//P0 -st^3 + 2st^2 + (-st)
					x = ((-s * pow(t, 3)) + (pow(t, 2) * 2.0 * s) + (t * -s)) * a[0];
					y = ((-s * pow(t, 3)) + (pow(t, 2) * 2.0 * s) + (t * -s)) * a[1];
					z = ((-s * pow(t, 3)) + (pow(t, 2) * 2.0 * s) + (t * -s)) * a[2];

					
					//

					//P1 (2-s)(t^3) + (s-3)(t^2) + 1
					x += ((pow(t, 3) * (2.0 - s)) + (pow(t, 2) * (s - 3.0)) + 1.0) * b[0];
					y += ((pow(t, 3) * (2.0 - s)) + (pow(t, 2) * (s - 3.0)) + 1.0) * b[1];
					z += ((pow(t, 3) * (2.0 - s)) + (pow(t, 2) * (s - 3.0)) + 1.0) * b[2];

					//

					//P2 (s-2)(t^3) + (3-2s)(t^2) + st
					x += ((pow(t, 3) * (s - 2.0)) + (pow(t, 2) * (3.0 - (2.0 * s))) + (s * t)) * c[0];
					y += ((pow(t, 3) * (s - 2.0)) + (pow(t, 2) * (3.0 - (2.0 * s))) + (s * t)) * c[1];
					z += ((pow(t, 3) * (s - 2.0)) + (pow(t, 2) * (3.0 - (2.0 * s))) + (s * t)) * c[2];

					//P3 s(t^3) + (-s)(t^2)
					x += ((pow(t, 3) * s) + (pow(t, 2) * -s)) * d[0];
					y += ((pow(t, 3) * s) + (pow(t, 2) * -s)) * d[1];
					z += ((pow(t, 3) * s) + (pow(t, 2) * -s)) * d[2];
					
					glVertex3f(x, y, z);

					vecX = oldx - x;
					vecY = oldy - y;
					vecZ = oldz - z;

					vecX = pow(vecX, 2);
					vecY = pow(vecY, 2);
					vecZ = pow(vecZ, 2);

					holdArcLen += sqrt(vecX + vecY + vecZ);

					//animTcl::OutputMessage("%d", holdArcLen);

					oldx = x;
					oldy = y;
					oldz = z;

				}

				ArcLen[i] = holdArcLen/40.0;

				glEnd();

				if (i + 3 == nextPoint && chkpt == 1) {
					chkpt = -1;
				}

			}

		}

	}

	glPopMatrix();
	glPopAttrib();
}	// SampleParticle::display