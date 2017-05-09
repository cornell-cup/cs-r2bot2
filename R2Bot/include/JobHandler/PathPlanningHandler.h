#ifndef _PATH_PLANNING_HANDLER_HANDLER
#define _PATH_PLANNING_HANDLER_HANDLER

#include "JobHandler.h"
#include "Data/UltrasoundData.h"
#include "PathPlanning/Grid.h"
#include "PathPlanning/Dijkstra.h"
#include "PathPlanning/Path.h"
#include "Data/MotorData.h"

class PathPlanningHandler : public JobHandler
{

private:
	Grid g;
	Dijkstra shortPath;
	Path path;
	int track;

public:
	PathPlanningHandler();
	PathPlanningHandler(float w, float h, float scal, float startx, float starty, float startAngle, float goalx, float goaly);
	~PathPlanningHandler();

	virtual void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif
