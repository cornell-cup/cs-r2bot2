#ifndef _R2BOT_LIDAR_SENSOR
#define _R2BOT_LIDAR_SENSOR

#include "Global.h"
#include <mutex>
#include <thread>
#include "Sensor.h"
#include "SerialPort.h"
#include "rplidar.h"
#include "R2Protocol.hpp"

#define NODE_COUNT 360 * 2
class LidarSensor : public Sensor {
protected:
	rp::standalone::rplidar::RPlidarDriver * driver;
	std::mutex dataMutex;
	std::thread bg_thread;
	bool bg_done = false;
	void bg_scan();
	std::vector<double> angles;
	std::vector<double>distances;
	ptr<SerialPort> conn;
public:
	LidarSensor(string port, int baudrate);
	~LidarSensor();

	bool ping();

	void fillData(SensorData& sensorData);

};

#endif
