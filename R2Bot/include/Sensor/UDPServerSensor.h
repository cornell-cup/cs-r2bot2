#ifndef _R2BOT_UDPSERVER_SENSOR
#define _R2BOT_UDPSERVER_SENSOR

#include "Global.h"
#include "R2Protocol.hpp"
#include "Sensor.h"
#include "UDPSocketServer.h"

#include <mutex>
#include <string>
#include <vector>

class UDPServerSensor : public Sensor {
protected:
	ptr<UDPSocketServer> server;
	std::mutex dataMutex;
	smap<ptr<void>> dataReceived;
	smap<std::vector<R2Protocol::Packet>> dataToForward;
public:
	UDPServerSensor(std::string port, int baudrate);
	~UDPServerSensor();

	bool ping();

	void fillData(SensorData& sensorData);
};

#endif