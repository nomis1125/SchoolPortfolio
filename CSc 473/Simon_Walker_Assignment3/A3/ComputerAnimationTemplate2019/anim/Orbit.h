#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "Dots.h"

#include <string>

// Simulates the orbit of an object

class Orbit : public BaseSimulator
{
public:

	Orbit(const std::string& name, Dots* target);
	~Orbit();
	int step(double time);
	int init(double time)
	{
		m_object->getState(m_pos0, 0, 0);
		setVector(m_vel0, 0, 0, 0);
		return 0;
	};

	int command(int argc, myCONST_SPEC char** argv);

protected:

	Vector m_pos0; // initial position
	Vector m_vel0; // initial velocity
	Vector m_pos;
	Vector m_vel;

	float epsilon;
	float maxSP;

	float link1;
	float link2;

	float endx;
	float endy;

	float th1;
	float th2;

	VectorObj posHold;
	Vector OgStart;

	float oldTime;

	float holdTimer;

	int holA;

	std::string textFile;

	Dots* m_object;

};