#include "Controller.h"

#include <string>

using std::string;

Controller::Controller(string controllerName) : name(controllerName)
{
}

Controller::~Controller()
{
}

string Controller::getName()
{
	return name;
}

bool Controller::ping()
{
	return false;
}

void Controller::sendData(ControllerData& controllerData) {
}
