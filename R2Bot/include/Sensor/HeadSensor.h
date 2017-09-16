#ifndef _R2BOT_HEAD_SENSOR
#define _R2BOT_HEAD_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "Controller.h"
#include "SerialPort.h"

#include <mutex>
#include <string>

class HeadSensor : public Controller, public Sensor {
protected:
	std::mutex dataMutex;
	ptr<SerialPort> conn;
public:
	HeadSensor(std::string port, int baudrate);
	~HeadSensor();

	bool ping();

	void fillData(SensorData& sensorData);
	void sendData(ControllerData& controllerData);
};

#endif