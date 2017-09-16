#ifndef _R2BOT_IMU_SENSOR
#define _R2BOT_IMU_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"

#include <mutex>
#include <string>

class IMUSensor : public Sensor {
protected:
	std::mutex dataMutex;
	ptr<SerialPort> conn;
public:
	IMUSensor(std::string port, int baudrate);
	~IMUSensor();

	bool ping();

	void fillData(SensorData& sensorData);
};

#endif