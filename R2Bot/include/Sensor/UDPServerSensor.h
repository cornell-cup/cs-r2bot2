#ifndef _R2BOT_UDPSERVER_SENSOR
#define _R2BOT_UDPSERVER_SENSOR

#include "Global.h"
#include "Sensor.h"
#include "Util/UDPSocketServer.h"

class UDPServerSensor : public Sensor {
protected:
	ptr<UDPSocketServer> server;
public:
	UDPServerSensor(string host, int port);
	~UDPServerSensor();

	bool ping();

	void getData(smap<void*> sensorData);
};

#endif