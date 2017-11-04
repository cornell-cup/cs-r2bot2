#define _USE_MATH_DEFINES

#include "JobHandler/PathPlanningHandler.h"
#include <cmath>
#include <iostream>
#include <iomanip>
using namespace std;

bool PathPlanningHandler::registered = JobHandler::RegisterJobHandler("path-planning", [](string command) {
	return (ptr<JobHandler>) std::make_shared<PathPlanningHandler>();
});

PathPlanningHandler::PathPlanningHandler()
{
	g = Grid(1000, 1000, 15, 13, 13, 0);
	shortPath = Dijkstra(&g);
	path = Path();
	track = 0;
}

PathPlanningHandler::PathPlanningHandler(float w, float h, float scal, float startx, float starty, float startAngle, float glx, float gly)
{
	g = Grid(w, h, scal, startx, starty, startAngle);
	shortPath = Dijkstra(&g);
	goalx = glx;
	goaly = gly;
	path = Path();
	track = 0;
}

PathPlanningHandler::PathPlanningHandler(FakeMapSensor &map, float glx, float gly)
{
	g = map.g;
	shortPath = Dijkstra(&g);
	goalx = glx;
	goaly = gly;
	path = shortPath.calcPath(goalx, goaly);
	track = 1;

}


PathPlanningHandler::~PathPlanningHandler()
{
}

void PathPlanningHandler::execute(deque<Job>& jobs, SensorData & data, ControllerData & outputs)
{
	if (!path.path.empty()) {
		//Extract next coordinate from shortest path and calculate motor commands
		Coord moveTo = path.path.back();
		path.path.pop_back();
		track++;
		g.updatePos(moveTo.row, moveTo.column);
		print();
	}
	else {
		//Stop motors if no path or goal reached
		cout << "Done" << endl;
	}
	/*
	Obstacle newObs;
	auto usInfo = data.find("ULTRASOUNDF");
	if (usInfo != data.end()) {
		//Add obstacle information to grid
		for (int i = 1; i <= 7; i++) {
			float inches = static_pointer_cast<UltrasoundData>(usInfo->second)->distance[i - 1];
			//NEED TO FIX THIS: newObs = Obstacle((float)(g.R2Pos->x), (float)(g.R2Pos->y), g.r2Angle, M_PI/4.f, inches);
			g.addObstacle(newObs);
		}
		//Recalculate path
		path = shortPath.calcPath(goalx, goaly);
		track = 0;
	}
	else if(track < path.path.size()){
		//Extract next coordinate from shortest path and calculate motor commands
		Coord moveTo = path.path[track];
		float xdist = moveTo.row - g.R2Pos->row; //NEED TO FIX THIS
		float ydist = moveTo.column - g.R2Pos->column;
		float radius = sqrt(pow(xdist, 2) + pow(ydist, 2));
		float angle = atan2(ydist, xdist) - g.r2Angle;
		int l = (int)(radius * cos(angle));
		int r = (int)(radius * sin(angle));
		track++;
		g.updatePos(moveTo.row, moveTo.column);
		ptr<MotorData> output = std::make_shared<MotorData>();
		output->leftMotor = l;
		output->rightMotor = r;
		outputs["MOTOR"] = output;
	}
	else {
		//Stop motors if no path or goal reached
		ptr<MotorData> output = std::make_shared<MotorData>();
		output->leftMotor = 0;
		output->rightMotor = 0;
		outputs["MOTOR"] = output;
	} */

}

void PathPlanningHandler::print()
{
	cout << "  ";
	for (int j = 0; j < g.map[0].size(); j++) {
		cout << setfill(' ') << setw(2) << internal;
		cout << j % 10;
	}
	cout << endl;
	for (int i = 0; i < g.map.size(); i++) {
		cout << setfill(' ') << setw(2) << internal;
		cout << i % 10 << " ";
		for (int j = 0; j < g.map[i].size(); j++) {
			cout << setfill(' ') << setw(2) << internal;
			if (g.map[i][j]->obst) {
				cout << "H ";
			}
			else if (find(path.path.begin(), path.path.end(), *(g.map[i][j])) != path.path.end()) {
				cout << "* ";
			}
			else {
				cout << "O ";
			}
		}
		cout << endl;
	}
	cout << "-----------" << endl;
}