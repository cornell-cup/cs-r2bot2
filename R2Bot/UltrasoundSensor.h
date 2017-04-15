#ifndef _R2BOT_ULTRASOUND_SENSOR
#define _R2BOT_ULTRASOUND_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SensorData.h"
#include "UDPSocketServer.h"

#include <mutex>

class UltrasoundSensor : public Sensor {
protected:
	ptr<UDPSocketServer> server;
	std::mutex dataMutex;
	smap<ptr<SensorData>> dataReceived;
	smap<R2Protocol::Packet> dataToForward;
public:
	UltrasoundSensor(string port, int baudrate);
	~UltrasoundSensor();

	bool ping();

	void virtual getData(smap<ptr<SensorData>>& sensorData);
};

#endif