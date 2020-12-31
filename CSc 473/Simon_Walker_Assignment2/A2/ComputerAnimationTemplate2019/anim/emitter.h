#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
//#include "BaseSystem.h"
#include "ballobj.h"
#include <string>

// Simulates the emitter of an object

class emitter : public BaseSimulator
{
public:

	emitter(const std::string& name, ballobj* target);
	~emitter();
	int emitter::command(int argc, myCONST_SPEC char** argv);
	int step(double time);
	int init(double time)
	{
		m_object->getStateArray(m_pos0, 0);
		m_object->getVelArray(m_vel0, 0);
		return 0;
	};

	//int command(int argc, myCONST_SPEC char** argv) { return TCL_OK; }

protected:

	Vector m_pos0; // initial position
	Vector m_vel0; // initial velocity
	Vector m_pos;
	Vector m_vel;

	Vector plane_Pos;//plane_Pose[0]=ks, plane_Pose[1]=pos on y, plane_Pose[2]=kd

	double g;//gravity
	double dt;//time step

	int numSprings;
	int posSprings;
	double saveSpr[6][100];//<index1> <index2> <ks> <kd> <restlength> extra

	int savePart[5000];

	double drag;

	int integrationType;

	ballobj* m_object;

};