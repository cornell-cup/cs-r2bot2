#ifndef _JobHendler
#define _JobHendler

#include <list>
#include "Sensor.h"
#include "Controller.h"

class Node {
	// TODO: define class
};

class JobHandler
{
protected:
	std::list<Sensor> sensors;
	std::list<Controller> controllers;
	std::list<Node> waypoints;

	/** Creates a path between two nodes and stores as Nodes in waypoints */
	virtual void pathPlan(Node startPos, Node endPos);

	/** Parses arguments from job string to values */
	virtual void parseArgs(std::string job);

public:
	JobHandler(std::list<Sensor> sensors, std::list<Controller> controllers);
	virtual ~JobHandler();

	/** Runs the job handler's actions */
	virtual void execute(std::string job);
};

#endif
