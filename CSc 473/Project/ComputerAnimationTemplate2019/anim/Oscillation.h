#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "SampleParticle.h"

#include <string>

// Simulates the Oscillation of an object

class Oscillation : public BaseSimulator
{
public:

	Oscillation(const std::string& name, SampleParticle* target);
	~Oscillation();

	void compute(int a, int b);

	int step(double time);
	int init(double time)
	{
		m_object->getState(m_pos0, 0);
		return 0;
	};

	int command(int argc, myCONST_SPEC char** argv) { return TCL_OK; }

protected:

	octTree m_pos0; // initial position

	Vector prevPos;

	int numbodies;

	SampleParticle* m_object;

};