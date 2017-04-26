#ifndef _R2BOT_DRAWER_SENSOR
#define _R2BOT_DRAWER_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SerialPort.h"

#include <mutex>

class DrawerSensor : public Sensor {
protected:
	ptr<SerialPort> conn;
	std::mutex dataMutex;
	smap<ptr<void>> dataReceived;
	smap<R2Protocol::Packet> dataToForward;
public:
	DrawerSensor(string port, int baudrate);
	~DrawerSensor();

	bool ping();

	void fillData(SensorData & sensorData);
	void sendData(ControllerData & controllerData);
};

#endif