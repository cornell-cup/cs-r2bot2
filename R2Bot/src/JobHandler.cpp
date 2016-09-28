#include "JobHandler.h"

JobHandler::JobHandler(std::list<Sensor> sensorList,
	std::list<Controller> controllerList) : sensors(sensorList),
	controllers(controllerList)
{
}

JobHandler::~JobHandler()
{
}

void JobHandler::pathPlan(Node startPos, Node endPos)
{
}

void JobHandler::parseArgs(std::string job)
{
}

void JobHandler::execute(std::string job)
{
}
