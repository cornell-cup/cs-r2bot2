#ifndef _R2BOT_RFID_SENSOR
#define _R2BOT_RFID_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"

#include <mutex>
#include <string>

class RFIDSensor : public Sensor {
protected:
	std::mutex dataMutex;
	ptr<SerialPort> conn;
public:
	RFIDSensor(std::string port, int baudrate);
	~RFIDSensor();

	bool ping();

	void fillData(SensorData& sensorData);
};

#endif