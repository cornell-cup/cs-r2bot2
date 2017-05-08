#ifndef _R2BOT_POWER_SENSOR
#define _R2BOT_POWER_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"

#include <mutex>

class PowerSensor : public Sensor {
protected:
	std::mutex dataMutex;
	ptr<SerialPort> conn;
public:
	PowerSensor(string port, int baudrate);
	~PowerSensor();

	bool ping();

	void fillData(SensorData& sensorData);
	void sendData(ControllerData & sensorData);
};

#endif