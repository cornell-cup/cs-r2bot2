#ifndef _R2BOT_ULTRASOUND_SENSOR
#define _R2BOT_ULTRASOUND_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"

#include <mutex>
#include <string>

class UltrasoundSensor : public Sensor {
protected:
	std::mutex dataMutex;
	ptr<SerialPort> connF;
	ptr<SerialPort> connB;
public:
	UltrasoundSensor(std::string port, int baudrate, std::string port2, int baudrate2);
	~UltrasoundSensor();

	bool ping();

	void fillData(SensorData& sensorData);
};

#endif