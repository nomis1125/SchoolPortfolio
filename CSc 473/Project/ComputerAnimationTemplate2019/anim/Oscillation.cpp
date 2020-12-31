#include "Oscillation.h"

Oscillation::Oscillation(const std::string& name, SampleParticle* target) :
	BaseSimulator(name),
	m_object(target)
{
}	// Oscillation

Oscillation::~Oscillation()
{
}	// Oscillation::~Oscillation

void Oscillation::compute(int a, int b) {
	double steps = 0.5;
	double bigG = 6.6743 * 15 * pow(10, -11); //Gravitational constant

	octTree A;//fist point
	octTree B;//next point

	bodyVec3 C;//old acceleration holder
	bodyVec3 D;//current acceleration holder
	octTree E;//oldpoint A

	bodyVec3 accA;
	bodyVec3 accB;

	for (int i = a; i < b; i++) {
		A = m_object->getState(A, i);
		accA = bodyVec3(0, 0, 0);
		E = A;//old A
		C = accA;//old Acc

		for (int k = i + 1; k < b; k++) {
			octTree vecAB;//vector between A and B and vel between the two

			B = m_object->getState(B, k);
			accB = m_object->getAcc(accB, k);

			//vector
			vecAB.pos.x = B.pos.x - A.pos.x;
			vecAB.pos.y = B.pos.y - A.pos.y;
			vecAB.pos.z = B.pos.z - A.pos.z;

			//vel
			vecAB.vel.x = B.vel.x - A.vel.x;
			vecAB.vel.y = B.vel.y - A.vel.y;
			vecAB.vel.z = B.vel.z - A.vel.z;

			double vecSq = (vecAB.pos.x * vecAB.pos.x) + (vecAB.pos.y * vecAB.pos.y) + (vecAB.pos.z * vecAB.pos.z);//squared
			double velSq = (vecAB.vel.x * vecAB.vel.x) + (vecAB.vel.y * vecAB.vel.y) + (vecAB.vel.z * vecAB.vel.z);



			double vec2 = sqrt(vecSq);
			double vec3 = vec2 * vecSq;//cubed

			if (vecSq == 0.0) {
				vecSq = 1.0;
			}

			double velvecSq = ((vecAB.pos.x * vecAB.vel.x) + (vecAB.pos.y * vecAB.vel.y) + (vecAB.pos.z * vecAB.vel.z)) / vecSq;

			bodyVec3 accAB = bodyVec3(0, 0, 0);//acceleration

			//Gravitational acceleration // Relation to Universal Law
			double BIGFORCE = 0.0;// -1 * bigG * A.mass * B.mass / vec2;
			//

			accAB.x = vecAB.pos.x / vec3;
			accAB.y = vecAB.pos.y / vec3;
			accAB.z = vecAB.pos.z / vec3;


			double maxAccAB = sqrt(pow(accAB.x, 2) + pow(accAB.y, 2) + pow(accAB.z, 2));

			if (maxAccAB < 8000) {
				accA.x += B.mass * accAB.x + BIGFORCE;
				accA.y += B.mass * accAB.y + BIGFORCE;
				accA.z += B.mass * accAB.z + BIGFORCE;

				accB.x -= A.mass * accAB.x + BIGFORCE;
				accB.y -= A.mass * accAB.y + BIGFORCE;
				accB.z -= A.mass * accAB.z + BIGFORCE;
			}

			m_object->setAcc(accA, i);
			m_object->setAcc(accB, k);
		}

		//A  C  D  E	

		D = m_object->getAcc(D, i);

		//vel
		A.vel.x = E.vel.x + (C.x + D.x) * steps / 2;
		A.vel.y = E.vel.y + (C.y + D.y) * steps / 2;
		A.vel.z = E.vel.z + (C.z + D.z) * steps / 2;

		//pos
		A.pos.x = E.pos.x + (E.vel.x + A.vel.x) * steps / 2 + (C.x - D.x) * steps * steps / 12;
		A.pos.y = E.pos.y + (E.vel.y + A.vel.y) * steps / 2 + (C.y - D.y) * steps * steps / 12;
		A.pos.z = E.pos.z + (E.vel.z + A.vel.z) * steps / 2 + (C.z - D.z) * steps * steps / 12;

		m_object->setState(A, i);
		m_object->setAcc(D, i);
	}
}

int Oscillation::step(double time)
{

	numbodies = m_object->getNum();
	int i = 0;
	int ik = numbodies / 10;

	//5000 particles. Comment out when using less.
	//compute(0, numbodies);
	
	compute(0, 200);
	compute(201, 400);
	compute(401, 600);
	compute(601, 800);
	compute(801, 1000);

	
	compute(1001, 1200);
	compute(1201, 1400);
	compute(1401, 1600);
	compute(1601, 1800);
	compute(1801, 2000);
	
	compute(2001, 2200);
	compute(2201, 2400);
	compute(2401, 2600);
	compute(2601, 2800);
	compute(2801, 3000);

	compute(3001, 3200);
	compute(3201, 3400);
	compute(3401, 3600);
	compute(3601, 3800);
	compute(3801, 4000);

	compute(4001, 4200);
	compute(4201, 4400);
	compute(4401, 4600);
	compute(4601, 4800);
	compute(4801, 5000);
	

	return 0;

}	// Oscillation::step