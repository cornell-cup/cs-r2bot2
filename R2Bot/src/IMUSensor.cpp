#include "Sensor/IMUSensor.h"
#include "Data/IMUData.h"
#include <iostream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

IMUSensor::IMUSensor(string port, int baudrate) : Sensor("IMU Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("IMU sensor connected to port %s\n", port.c_str());
}

IMUSensor::~IMUSensor() {
}

bool IMUSensor::ping() {
	return conn->isConnected() == 1;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

void IMUSensor::fillData(SensorData& sensorData) {
	if (conn->isConnected()) {
		char data[256];
		int bytesRead = conn->read(data, 256);
		if (bytesRead <= 0) {
			return;
		}
		std::lock_guard<std::mutex> lock(dataMutex);
		std::vector<uint8_t> input(data, data + bytesRead);
		ptr<IMUData> idata = std::make_shared<IMUData>();
		string imuData(input.begin(), input.end());
		string tImuData = rtrim(imuData);
		if (tImuData.length() != 0) {
			idata->xDirection = std::stof(tImuData);
			sensorData["IMU"] = idata;
			printf("IMU X Position: %f\n", idata->xDirection);
		}
	}
}