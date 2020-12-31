#include "tankPath.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

tankPath::tankPath(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	nowpos(0),
	chkpt(0),
	bigCurrPt(0)
{

	setVector(m_pos, 0, 0, 0);
	curveType = 0.0f;
}	// tankPath

void tankPath::getSpeed(double t)
{

	//double speed = ArcLen[bigCurrPt]/t;//distance over time
	double speed = t*1000;

	animTcl::OutputMessage("Speed: %f m/s", speed);
	

}	// tankPath::getState

void tankPath::getState(double* p)
{

	VecCopy(p, m_pos);

}	// tankPath::getState

void tankPath::setState(double* p)
{

	VecCopy(m_pos, p);

}	// tankPath::setState

void tankPath::getHoldPoints2Len(int p)
{

	p = chkpt;

}	// tankPath::setState

void tankPath::getCurrPos(int p)
{

	p = nowpos;

}	// tankPath::setState

void tankPath::getStateArray(double* p, int i)
{

	VecCopy(p, holdPoints[i]);

}	// SampleParticle::getState

void tankPath::setStateArray(double* p, int i)
{

	VecCopy(holdPoints[i], p);

}	// SampleParticle::setState

void tankPath::getStateArray2(double* p)
{

	VecCopy(p, holdPoints2[nowpos]);

}	// SampleParticle::getState

void tankPath::setStateArray2(double* p, int i)
{

	VecCopy(holdPoints2[i], p);

}	// SampleParticle::setState
void tankPath::nextStateArray2()
{
	nowpos++;

}	// SampleParticle::setState

void tankPath::getTangentArray2(double* p)
{

	VecCopy(p, holdTangents2[nowpos+1]);

}	// SampleParticle::getState

void tankPath::setTangentArray2(double* p, int i)
{

	VecCopy(holdTangents2[i], p);

}	// SampleParticle::setState

void tankPath::nextbigCurrPt()
{
	bigCurrPt++;

}	// SampleParticle::setState

void tankPath::getTangentArray(double* p, int i)
{

	VecCopy(p, holdTangent[i]);

}	// SampleParticle::getState

void tankPath::setTangentArray(double* p, int i)
{

	VecCopy(holdTangent[i], p);

}	// SampleParticle::setState

void tankPath::hermiteSpline()
{

	if (currPoint >= 4) {
		int nextPoint = currPoint;
		//int chkpt = 0;//made for pos in array
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
				getStateArray(c, i + 1);//point B
				getTangentArray(d, i + 1);

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
					double x1 = (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[0];
					double y1 = (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[1];
					double z1 = (pow(t, 3) - (2.0 * pow(t, 2)) + t) * b[2];

					//t += 0.025f;
					Vector holdtan;
					holdtan[0] = x1;
					holdtan[1] = y1;
					holdtan[2] = z1;
					setTangentArray2(holdtan, chkpt);

					x += x1;
					y += y1;
					z += z1;

					//P2 || f2(t)
					x += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[0];
					y += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[1];
					z += ((-2.0 * pow(t, 3)) + (3.0 * pow(t, 2))) * c[2];

					//P2' || f4(t)
					x += (pow(t, 3) - pow(t, 2)) * d[0];
					y += (pow(t, 3) - pow(t, 2)) * d[1];
					z += (pow(t, 3) - pow(t, 2)) * d[2];

					Vector newXYZ;
					newXYZ[0] = x;
					newXYZ[1] = y;
					newXYZ[2] = z;

					setStateArray2(newXYZ, chkpt);
					chkpt++;
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
				ArcLen[i] = holdArcLen / 40.0;

			}
		}
	}	// tankPath::Reset
}

void tankPath::reset(double time)
{
	setVector(m_pos, 0, 0, 0);
	currPoint = 0;
	nowpos = 0;
	bigCurrPt = 0;
	Vector a;
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	for (int i = 0; i < 40; i++) {
		setStateArray(a, i);
		setTangentArray(a, i);
		ArcLen[i] = 0.0;
	}

}	// tankPath::Reset


int tankPath::command(int argc, myCONST_SPEC char** argv)
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
	else if (strcmp(argv[0], "flipNormals") == 0)
	{
		flipNormals();
		return TCL_OK;

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
					line = line.substr(prev.length() + 1);
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
			hermiteSpline();
		}
		else {
			animTcl::OutputMessage("Usage: system <name> load \"< file name >\"");
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

}	// tankPath::command

void tankPath::display(GLenum mode) //start new beginning
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

}	// tankPath::display
