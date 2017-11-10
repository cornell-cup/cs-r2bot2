#ifndef _R2BOT_LIDAR_SENSOR
#define _R2BOT_LIDAR_SENSOR

#include "Global.h"
#include "Sensor.h"
#include "SerialPort.h"
#include "rplidar.h"
#include "R2Protocol.hpp"

#include <mutex>
#include <string>
#include <thread>

#define NODE_COUNT 360 * 2
class LidarSensor : public Sensor {
protected:
	ptr<SerialPort> conn;
	std::mutex dataMutex;
	rp::standalone::rplidar::RPlidarDriver * driver;
	std::thread bg_thread;
	bool bg_done = false;
	void bg_scan();
	std::vector<double> angles;
	std::vector<double>distances;
public:
	LidarSensor(std::string port, int baudrate);
	~LidarSensor();

	bool ping();

	void fillData(SensorData& sensorData);

};

#endif
