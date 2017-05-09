#include "JobHandler/PathPlanningHandler.h"
#include <cmath>
using namespace std;

bool PathPlanningHandler::registered = JobHandler::RegisterJobHandler("path-planning", [](string command) {
	return (ptr<JobHandler>) std::make_shared<PathPlanningHandler>();
});

PathPlanningHandler::PathPlanningHandler()
{
	g = Grid(1000, 1000, 13, 13, 13, 0);
	shortPath = Dijkstra(500, 500);
	path = Path();
	track = 0;
}

PathPlanningHandler::PathPlanningHandler(float w, float h, float scal, float startx, float starty, float startAngle, float goalx, float goaly)
{
	g = Grid(w, h, scal, startx, starty, startAngle);
	shortPath = Dijkstra(goalx, goaly);
	path = Path();
	track = 0;
}


PathPlanningHandler::~PathPlanningHandler()
{
}

void PathPlanningHandler::execute(deque<Job>& jobs, SensorData & data, ControllerData & outputs)
{
	Obstacle newObs;
	auto usInfo = data.find("ULTRASOUNDF");
	if (usInfo != data.end()) {
		for (int i = 1; i <= 7; i++) {
			float inches = static_pointer_cast<UltrasoundData>(usInfo->second)->distance[i - 1];
			newObs = Obstacle(g.R2Pos.x, g.R2Pos.y, g.r2Angle, 3.14159265/4, inches);
			g.addObstacle(newObs);
		}
		path = shortPath.calcPath(&g);
		track = 0;
	}
	else {
		Coord moveTo = path.path[track];
		float xdist = moveTo.x - g.R2Pos.x;
		float ydist = moveTo.y - g.R2Pos.y;
		float radius = sqrt(pow(xdist, 2) + pow(ydist, 2));
		float angle = atan2(ydist, xdist) - g.r2Angle;
		int l = (int)(radius * cos(angle));
		int r = (int)(radius * sin(angle));
		track++;
		g.updatePos(moveTo.x, moveTo.y);
		ptr<MotorData> output = std::make_shared<MotorData>();
		output->leftMotor = l;
		output->rightMotor = r;
		outputs["MOTOR"] = output;
		
	}
}
