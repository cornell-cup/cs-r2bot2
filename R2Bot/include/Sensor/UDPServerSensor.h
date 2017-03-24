#ifndef _R2BOT_UDPSERVER_SENSOR
#define _R2BOT_UDPSERVER_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "SensorData.h"
#include "UDPSocketServer.h"

#include <mutex>

class UDPServerSensor : public Sensor {
protected:
	ptr<UDPSocketServer> server;
	std::mutex dataMutex;
	smap<ptr<SensorData>> dataReceived;
	smap<R2Protocol::Packet> dataToForward;
public:
	UDPServerSensor(string host, int port);
	~UDPServerSensor();

	bool ping();

	void getData(smap<ptr<SensorData>>& sensorData);
};

#endif