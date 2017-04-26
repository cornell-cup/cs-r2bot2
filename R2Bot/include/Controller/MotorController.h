#ifndef _R2BOT_MOTOR_CONTROLLER
#define _R2BOT_MOTOR_CONTROLLER

#include "Global.h"
#include "Controller.h"
#include "SerialPort.h"

class MotorController : public Controller {
protected:
	ptr<SerialPort> conn;
public:
	MotorController(string port, int baudrate);
	virtual ~MotorController();

	virtual bool ping();

	virtual void sendData(smap<ptr<void>>& data);
};

#endif