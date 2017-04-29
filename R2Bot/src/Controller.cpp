#include "Controller.h"

Controller::Controller(std::string controllerName) : name(controllerName)
{
}

Controller::~Controller()
{
}

std::string Controller::getName()
{
	return name;
}

bool Controller::ping()
{
	return false;
}

void Controller::sendData(ControllerData& controllerData) {
}
