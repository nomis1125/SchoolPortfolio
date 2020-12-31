#include "emitter.h"
#include <math.h>

emitter::emitter(const std::string& name, ballobj* target) :
	BaseSimulator(name),
	m_object(target),
	numSprings(0),
	integrationType(0),
	posSprings(0),
	drag(1)
{
	g = -9.8;
	dt = 0.01;
	
}	// emitter

emitter::~emitter()
{
}	// emitter::~emitter

int emitter::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "link") == 0)// Sets the total amount of springs
	{
		if (argc == 3)
		{
			numSprings = atof(argv[2]);

			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: dim <Number of Particles>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "spring") == 0)// Creates springs between two particles
	{
		if (argc == 6)
		{
			saveSpr[0][posSprings] = atof(argv[1]);//index1
			saveSpr[1][posSprings] = atof(argv[2]);//index2
			saveSpr[2][posSprings] = atof(argv[3]);//ks
			saveSpr[3][posSprings] = atof(argv[4]);//kd

			//All below for restlength
			Vector a;
			Vector b;
			m_object->getStateArray(a, atof(argv[1]));
			m_object->getStateArray(b, atof(argv[2]));

			double xa = sqrt(pow(a[0]-b[0], 2) + pow(a[1]-b[1], 2) + pow(a[2]-b[2], 2));

			if (atof(argv[5]) < 0.0) {
				if (xa < 0) {
					saveSpr[4][posSprings] = 1;
				}
				else {
					saveSpr[4][posSprings] = xa;
				}
			}
			else {
				saveSpr[4][posSprings] = atof(argv[5]);
			}
			saveSpr[5][posSprings] = 1;

			posSprings++;

			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: dim <Number of Particles>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "fix") == 0)// Fixes a particle to its position
	{
		if (argc == 2)
		{
			int pos = atof(argv[1]);
			savePart[pos] = 0;
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: dim <Number of Particles>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "integration") == 0)// Chooses an integrationType
	{
		if (argc == 3)
		{
			dt = atof(argv[2]);

			if (strcmp(argv[1], "euler") == 0) { //x(t) + hv(x(t), t)
				integrationType = 0;
			}
			else if (strcmp(argv[1], "symplectic") == 0) {
				integrationType = 1;
			}
			else if (strcmp(argv[1], "verlet") == 0) {
				integrationType = 2;
			}
			else {
				animTcl::OutputMessage("Usage: integration <euler|symplectic|verlet> <time step>");
				return TCL_ERROR;
			}
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: integration <euler|symplectic|verlet> <time step>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "ground") == 0)// Fixes a particle to its position
	{
		if (argc == 3)
		{
			plane_Pos[0] = atof(argv[1]);//ks = spring constant
			plane_Pos[1] = -10.0;
			plane_Pos[2] = atof(argv[2]);//kd = spring damper
		}
		else
		{
			animTcl::OutputMessage("Usage: ground <ks> <kd>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "gravity") == 0)// Fixes a particle to its position
	{
		if (argc == 2)
		{
			g = atof(argv[1]);
		}
		else
		{
			animTcl::OutputMessage("Usage: gravity <g>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "drag") == 0)// Fixes a particle to its position
	{
	if (argc == 2)
	{
		drag = atof(argv[1]);
		
		return TCL_OK;
	}
	else
	{
		animTcl::OutputMessage("Usage: simulator <sim_name> drag <kdrag>");
		return TCL_ERROR;
	}
	}

	glutPostRedisplay();
	return TCL_OK;

}	// ballobj::command

int emitter::step(double time)
{
	Vector pos;
	Vector vel;

	int itsaSpring = 0;

	double verletPosY = 0.0;
	double verletVelY = 0.0;

	int numPart = m_object->getPartNum();
	
	int k = 0;
	for (int i = 0; i < numPart; i++) {

		m_object->getStateArray(pos, i);
		m_object->getVelArray(vel, i);
		float mass = 1.0;
		m_object->getMass(mass, i);
		double oldYVel = vel[1];

		if (integrationType == 0) {// Forward Euler

			vel[0] = vel[0];
			vel[1] = (vel[1] + g * dt / mass);
			vel[2] = vel[2];
			
			pos[0] = pos[0] + vel[0] * dt;
			pos[1] = pos[1] + oldYVel * dt;
			pos[2] = pos[2] + vel[2] * dt;
		}
		else if (integrationType == 1) {// Symplectic Euler-

			vel[0] = vel[0];
			vel[1] = (vel[1] + g * dt / mass);
			vel[2] = vel[2];

			pos[0] = pos[0] + vel[0] * dt;
			pos[1] = pos[1] + vel[1] * dt;
			pos[2] = pos[2] + vel[2] * dt;
		}
		else if (integrationType == 2) {//verlet

			if (i == 0) {
				vel[1] = (vel[1] + g * dt / mass);
				
				pos[0] = pos[0] + vel[0] * dt;
				pos[1] = pos[1] + vel[1] * dt;
				pos[2] = pos[2] + vel[2] * dt;

			}
			else {
				pos[0] = pos[0] + vel[0] * dt;
				pos[1] = 2 * pos[1] - verletPosY + g * dt * dt / mass;
				pos[2] = pos[2] + vel[2] * dt;

				vel[0] = vel[0];
				vel[1] = (pos[1] - verletPosY) / 2 * dt;
				vel[2] = vel[2];

			}

			verletPosY = pos[1];
			verletVelY = vel[1];
		}

		//Spring force applyer
		double forcex = 0.0;//Fsp
		double forcey = 0.0;//Fsp
		double forcez = 0.0;//Fsp
		
		for (int m = 0; m < numSprings; m++) {
			itsaSpring = 1;
			if (saveSpr[0][m] == i) {
				Vector connected;
				Vector connectedVel;
				m_object->getStateArray(connected, saveSpr[1][m]);
				m_object->getVelArray(connectedVel, saveSpr[1][m]);

				double subNumsx = 1.0;
				double subNumsy = 1.0;
				double subNumsz = 1.0;

				if (pos[0] - connected[0] != 0.0) {
					subNumsx = pos[0] - connected[0];
				}
				if (pos[1] - connected[1] != 0.0) {
					subNumsy = pos[1] - connected[1];
				}
				if (pos[2] - connected[2] != 0.0) {
					subNumsz = pos[2] - connected[2];
				}

				//j-i
				// l = sqrt ((xi - xj )^2 + (yi - yj )^2 + (zi - zj )^2)
				// F = ks * (l - restL)
				// x = F * (xi - xj) / |(xi-xj)|...
				double currentPos = sqrt(pow(pos[0] - connected[0], 2.0) + pow(pos[1] - connected[1], 2.0) + pow(pos[2] - connected[2], 2.0));

				if (time > 1) {
					double check = 0.0;
					check++;
				}

				//Calculate Fsp
				double marcForce = saveSpr[2][m] * (saveSpr[4][m] - currentPos);
				forcex += marcForce * (pos[0] - connected[0]) / currentPos;
				forcey += marcForce * (pos[1] - connected[1]) / currentPos;
				forcez += marcForce * (pos[2] - connected[2]) / currentPos;

				//Calculate Fd
				forcex -= saveSpr[3][m] * ((connectedVel[0] - vel[0]) * (pos[0] - connected[0]) / currentPos) * (pos[0] - connected[0]) / currentPos;
				forcey -= saveSpr[3][m] * ((connectedVel[1] - vel[1]) * (pos[1] - connected[1]) / currentPos) * (pos[1] - connected[1]) / currentPos;
				forcez -= saveSpr[3][m] * ((connectedVel[2] - vel[2]) * (pos[2] - connected[2]) / currentPos) * (pos[2] - connected[2]) / currentPos;

			}
			if (saveSpr[1][m] == i) {
				Vector connected;
				Vector connectedVel;
				m_object->getStateArray(connected, saveSpr[0][m]);
				m_object->getVelArray(connectedVel, saveSpr[0][m]);

				double subNumsx = 1.0;
				double subNumsy = 1.0;
				double subNumsz = 1.0;

				if (time > 1 && (i == 0 || i == 1 || i == 6 || i == 7)) {
					double check = 0.0;
					check++;
				}

				if (connected[0]- pos[0] != 0.0) {
					subNumsx = connected[0] - pos[0];
				}
				if (connected[1] - pos[1] != 0.0) {
					subNumsy = connected[1] - pos[1];
				}
				if (connected[2] - pos[2] != 0.0) {
					subNumsz = connected[2] - pos[2];
				}

				double currentPos = sqrt(pow(connected[0] - pos[0], 2) + pow(connected[1] - pos[1], 2) + pow(connected[2] - pos[2], 2));

				//Calculate Fsp
				double marcForce = saveSpr[2][m] * (currentPos - saveSpr[4][m]);
				forcex += marcForce * (connected[0] - pos[0]) / currentPos;
				forcey += marcForce * (connected[1] - pos[1]) / currentPos;
				forcez += marcForce * (connected[2] - pos[2]) / currentPos;

				//Calculate Fd
				forcex -= saveSpr[3][m] * (vel[0] - connectedVel[0]) * (connected[0] - pos[0]) / currentPos * (connected[0] - pos[0]) / currentPos;
				forcey -= saveSpr[3][m] * (vel[1] - connectedVel[1]) * (connected[1] - pos[1]) / currentPos * (connected[1] - pos[1]) / currentPos;
				forcez -= saveSpr[3][m] * (vel[2] - connectedVel[2]) * (connected[2] - pos[2]) / currentPos * (connected[2] - pos[2]) / currentPos;
				
			}
			
		}

		if (itsaSpring == 1) {
			vel[0] = vel[0] + forcex * dt / mass;
			vel[1] = vel[1] + forcey * dt / mass;
			vel[2] = vel[2] + forcez * dt / mass;
			
		}

		vel[0] = vel[0] * drag;
		vel[1] = vel[1] * drag;
		vel[2] = vel[2] * drag;
		
		if (pos[1] < plane_Pos[1]) {//checks if it goes under the floor
			
			//                   -drag     vel     grav and mass     Fext           Fspring                FDamp        
			double force = -1.0 * drag * oldYVel + g * dt * mass + forcey + (-1.0 * plane_Pos[0]) + (-1.0 * plane_Pos[2] * oldYVel);

			//double hold = plane_Pos[0] * -0.001;//ks
			vel[1] = oldYVel + force / mass * dt;//vel[1] * hold;
			pos[1] = plane_Pos[1] + vel[1] * dt;
		}

		for (int m = 0; m < numSprings; m++) {

			if (saveSpr[0][m] == i) {//i) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_COLOR_MATERIAL);//keep
				glBegin(GL_LINES);

				glColor3f(1.0, 0.0, 0.0);
				glVertex3f(pos[0], pos[1], pos[2]);

				Vector m_posi;
				m_object->getStateArray(m_posi, saveSpr[1][m]);
				glVertex3f(m_posi[0], m_posi[1], m_posi[2]);

				glEnd;

				}
		}

		if (savePart[i] != 0) {
			m_object->setVelArray(vel, i);
			m_object->setStateArray(pos, i);
		}
	}

	return 0;

}	// emitter::step
/*
//Calculate Fsp
				forcex -= saveSpr[2][m] * (abs(connected[0] - pos[0]) - saveSpr[4][m]) * (connected[0] - pos[0]) / abs(subNumsx);
				forcey -= saveSpr[2][m] * (abs(connected[1] - pos[1]) - saveSpr[4][m]) * (connected[1] - pos[1]) / abs(subNumsy);
				forcez -= saveSpr[2][m] * (abs(connected[2] - pos[2]) - saveSpr[4][m]) * (connected[2] - pos[2]) / abs(subNumsz);

				//Calculate Fd
				//-1.0 *
				forcex -= -1.0 * saveSpr[3][m] * ((connectedVel[0] - vel[0]) * ((connected[0] - pos[0]) / abs(subNumsx))) * (connected[0] - pos[0]) / abs(subNumsx);
				forcey -= -1.0 * saveSpr[3][m] * ((connectedVel[1] - vel[1]) * ((connected[1] - pos[1]) / abs(subNumsy))) * (connected[1] - pos[1]) / abs(subNumsy);
				forcez -= -1.0 * saveSpr[3][m] * ((connectedVel[2] - vel[2]) * ((connected[2] - pos[2]) / abs(subNumsy))) * (connected[2] - pos[2]) / abs(subNumsz);

*/