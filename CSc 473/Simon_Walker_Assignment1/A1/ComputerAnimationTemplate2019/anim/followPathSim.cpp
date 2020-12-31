#include "followPathSim.h"

followPathSim::followPathSim(const std::string& name, tankPath* target, missilePath* target2) :
	BaseSimulator(name),
	m_object(target),
	m_object2(target2),
	prev(0),
	chktm(0)
{
}	// followPathSim

followPathSim::~followPathSim()
{
}	// followPathSim::~followPathSim

int followPathSim::step(double time)
{
	double g = -9.8;
	Vector pos;
	Vector arrayPos;
	Vector change;

	m_object->getState(pos);
	m_object->getStateArray2(arrayPos);

	VecCopy(change, arrayPos);

	int mop = 0;//size of array
	int curpo = 0;//current position in array
	m_object->getHoldPoints2Len(mop);
	m_object->getCurrPos(curpo);

	//easy in/out?
	if (curpo < mop * 0.1) {
		m_object->nextStateArray2();
	}
	else if (time > mop - (mop * 0.1)) {
		m_object->nextStateArray2();
	}
	else {
		m_object->nextStateArray2();
		m_object->nextStateArray2();
		m_object->nextStateArray2();
		m_object->nextStateArray2();
	}

	//speed
	if (prev == 100) {

		Vector a;
		Vector b;

		a[0] = pos[0];
		a[1] = pos[1];
		a[2] = pos[2];

		b[0] = arrayPos[0];
		b[1] = arrayPos[1];
		b[2] = arrayPos[2];

		double t = sqrt(pow(a[0]-b[0], 2) + pow(a[1]-b[1], 2) + pow(a[2]-b[2], 2));

		m_object->getSpeed(t);
		prev = 0;
	}
	prev++;
	chktm++;

	m_object->setState(arrayPos);
	//

	//  Missile:

	Vector pos1;
	Vector arrayPos1;
	Vector change1;

	m_object2->getState(pos1);//missile pos
	m_object->getTangentArray2(arrayPos1);//tan of object
	
	m_object2->positioning(pos1, arrayPos1, change, change1);//missile, tan, tank pos, save

	m_object2->setState(change1);
	
	return 0;

}	// followPathSim::step