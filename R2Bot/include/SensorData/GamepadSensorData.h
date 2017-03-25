#ifndef _R2BOT_GAMEPAD_SENSOR_DATA
#define _R2BOT_GAMEPAD_SENSOR_DATA

#include "Global.h"
#include "SensorData.h"

class GamepadSensorData : public SensorData {
protected:
public:
	GamepadSensorData(float x, float y);
	virtual ~GamepadSensorData();

	float x;
	float y;
};

#endif