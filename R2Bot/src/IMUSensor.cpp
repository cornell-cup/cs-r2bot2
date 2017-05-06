#include "Sensor/IMUSensor.h"
#include "Data/IMUData.h"
#include <iostream>

IMUSensor::IMUSensor(string port, int baudrate) : Sensor("IMU Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("IMU sensor connected to port %s\n", port.c_str());
}

IMUSensor::~IMUSensor() {
}

bool IMUSensor::ping() {
	return conn->isConnected() == 1;
}

void IMUSensor::fillData(SensorData& sensorData) {
	if (conn->isConnected()) {
		char data[256];
		int bytesRead = conn->read(data, 256);
		if (bytesRead <= 0) {
			return;
		}
		std::lock_guard<std::mutex> lock(dataMutex);
		// Decode the incoming data
		R2Protocol::Packet params;
		std::vector<uint8_t> input(data, data + bytesRead);
		int32_t read;
		ptr<IMUData> idata = std::make_shared<IMUData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if (params.source.c_str() == "IMU") {
				idata->xDirection = std::atof((char *)params.data.data());
			}
			idata->xDirection = std::atof((char *)params.data.data());
			std::vector<uint8_t> newinput(input.begin() + read, input.end());
			newinput.swap(input);
			sensorData["IMU"] = idata;
			printf("IMU X Position: %f\n", idata->xDirection);
		}
	}
}