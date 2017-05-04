#ifndef _R2BOT_RFID_SENSOR
#define _R2BOT_RFID_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"
#include "Controller.h"

#include <mutex>

class RFIDSensor : public Sensor {
protected:
	ptr<SerialPort> conn;
public:
	RFIDSensor(string port, int baudrate);
	~RFIDSensor();

	bool ping();

	void fillData(SensorData& sensorData);
	void sendData(ControllerData& controllerData);
};

#endif