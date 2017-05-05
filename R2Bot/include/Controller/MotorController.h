#ifndef _R2BOT_MOTOR_CONTROLLER
#define _R2BOT_MOTOR_CONTROLLER

#include "Global.h"
#include "Controller.h"
#include "SerialPort.h"

#define MOTOR_MAX_SPEED 300

class MotorController : public Controller {
protected:
	ptr<SerialPort> conn;

	/** Pad an integer into a string */
	static string _pad(int i, unsigned int l);
public:
	MotorController(string port, int baudrate);
	virtual ~MotorController();

	virtual bool ping();

	virtual void sendData(ControllerData& data);
};

#endif