#ifndef _PATH_PLANNING_HANDLER_HANDLER
#define _PATH_PLANNING_HANDLER_HANDLER

#include "JobHandler.h"
#include "Data/UltrasoundData.h"
#include "PathPlanning/Grid.h"
#include "PathPlanning/Dijkstra.h"
#include "PathPlanning/Path.h"
#include "Data/MotorData.h"
#include "PathPlanning/FakeMapSensor.h"

class PathPlanningHandler : public JobHandler
{

private:
	static bool registered;
	Grid g;
	Dijkstra shortPath;
	Path path;
	int track;
	float goalx;
	float goaly;

public:
	PathPlanningHandler();
	PathPlanningHandler(float w, float h, float scal, float startx, float starty, float startAngle, float goalx, float goaly);
	PathPlanningHandler(FakeMapSensor &map, float glx, float gly);
	~PathPlanningHandler();

	virtual void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
	void print();
};

#endif
