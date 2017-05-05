#ifndef _R2BOT_ULTRASOUND_SENSOR
#define _R2BOT_ULTRASOUND_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"

#include <mutex>

class UltrasoundSensor : public Sensor {
protected:
	std::mutex dataMutex;
	ptr<SerialPort> conn;
public:
	UltrasoundSensor(string port, int baudrate);
	~UltrasoundSensor();

	bool ping();

	void fillData(SensorData& sensorData);
};

#endif