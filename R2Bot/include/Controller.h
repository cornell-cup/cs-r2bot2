#ifndef _R2BOT_CONTROLLER
#define _R2BOT_CONTROLLER

#include "Global.h"

#include <string>

class Controller
{
protected:
	/** Unique name of the controller */
	std::string name;

public:
	Controller(std::string controllerName);
	virtual ~Controller();

	/** Returns the name of the controller */
	std::string getName();

	/** Returns true if can communicate with controller */
	virtual bool ping();

	/** Sends the controller-specific data to the controller */
	virtual void sendData(std::string data);
};

#endif
