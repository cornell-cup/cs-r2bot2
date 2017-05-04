#ifndef _R2BOT_HEAD_SENSOR
#define _R2BOT_HEAD_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"

#include <mutex>

class HeadSensor : public Sensor {
protected:
	ptr<SerialPort> conn;
public:
	HeadSensor(string port, int baudrate);
	~HeadSensor();

	bool ping();

	void fillData(SensorData& sensorData);
	void sendData(ControllerData& controllerData);
};

#endif