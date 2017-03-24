#ifndef _R2BOT_GAMEPAD_SENSOR_DATA
#define _R2BOT_GAMEPAD_SENSOR_DATA

#include "Global.h"
#include "SensorData.h"

class GamepadSensorData : public SensorData {
protected:
public:
	GamepadSensorData(int leftMotor, int rightMotor);
	virtual ~GamepadSensorData();

	int leftMotor;
	int rightMotor;
};

#endif