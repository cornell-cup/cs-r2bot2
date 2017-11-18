#ifndef _R2BOT_MOTOR_CONTROLLER
#define _R2BOT_MOTOR_CONTROLLER

#include "Global.h"
#include "Controller.h"
#include "SerialPort.h"

#define MOTOR_MAX_SPEED 250

class MotorController : public Controller {
protected:
	ptr<SerialPort> conn;

	/** Pad an integer into a string */
	static std::string _pad(int i, unsigned int l);

    int counter;
public:
	MotorController(std::string port, int baudrate);
	virtual ~MotorController();

	virtual bool ping();

	virtual void sendData(ControllerData& data);
};

#endif
