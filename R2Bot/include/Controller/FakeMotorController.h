#ifndef _R2BOT_FAKE_MOTOR_CONTROLLER
#define _R2BOT_FAKE_MOTOR_CONTROLLER

#include "Global.h"
#include "Controller.h"

class FakeMotorController : public Controller {
public:
	FakeMotorController();
	virtual ~FakeMotorController();

	virtual bool ping();

	virtual void sendData(ControllerData& data);
};

#endif