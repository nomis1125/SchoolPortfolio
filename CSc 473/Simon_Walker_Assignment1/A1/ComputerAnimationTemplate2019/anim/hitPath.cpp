#include "hitPath.h"

hitPath::hitPath(const std::string& name, missilePath* target) :
	BaseSimulator(name),
	m_object(target)
{
}	// hitPath

hitPath::~hitPath()
{
}	// hitPath::~hitPath

int hitPath::step(double time)
{
	double g = -9.8;
	Vector pos;
	Vector arrayPos;
	Vector change;

	m_object->getState(pos);
	m_object->getStateArray2(arrayPos);

	int mop = 0;//size of array
	int curpo = 0;//current position in array
	m_object->getHoldPoints2Len(mop);
	m_object->getCurrPos(curpo);

	m_object->nextStateArray2();

	m_object->setState(pos);

	return 0;

}	// hitPath::step