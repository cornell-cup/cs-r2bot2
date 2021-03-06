#ifndef _R2BOT_HEAD_FLAP_CONTROLLER
#define _R2BOT_HEAD_FLAP_CONTROLLER

#include "Global.h"
#include "Controller.h"
#include "SerialPort.h"

#include <string>

class HeadFlapController : public Controller {
protected:
	ptr<SerialPort> conn;

public:
	HeadFlapController(std::string port, int baudrate);
	virtual ~HeadFlapController();

	virtual bool ping();

	virtual void sendData(ControllerData& data);
};

#endif