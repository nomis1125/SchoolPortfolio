#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "tankPath.h"
#include "missilePath.h"

#include <string>

// Simulates the followPathSim of an object

class followPathSim : public BaseSimulator
{
public:

	followPathSim(const std::string& name, tankPath* target, missilePath* target2);
	~followPathSim();

	int step(double time);
	int init(double time)
	{
		m_object->getState(m_pos0);
		setVector(m_vel0, 0, 0, 0);
		return 0;
	};

	int command(int argc, myCONST_SPEC char** argv) { return TCL_OK; }

protected:

	Vector m_pos0; // initial position
	Vector m_vel0; // initial velocity
	Vector m_pos;
	Vector m_vel;
	int prev;
	int chktm;

	tankPath* m_object;
	missilePath* m_object2;

};