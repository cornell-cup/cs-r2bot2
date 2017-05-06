#include "Sensor/UltrasoundSensor.h"
#include "Data/UltrasoundData.h"
#include <iostream>

UltrasoundSensor::UltrasoundSensor(string port, int baudrate) : Sensor("Ultrasound Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("Ultrasound connected to port %s\n", port.c_str());
}

UltrasoundSensor::~UltrasoundSensor() {
}

bool UltrasoundSensor::ping() {
	return conn->isConnected() == 1;
}

void UltrasoundSensor::fillData(SensorData& sensorData) {
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
		ptr<UltrasoundData> udata = std::make_shared<UltrasoundData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if ((params.source).find("SENSOR")) {
				udata->distance = std::atof((char *)params.data.data());
			}
			udata->distance = std::atof((char *)params.data.data());
			std::vector<uint8_t> newinput(input.begin() + read, input.end());
			newinput.swap(input);
			sensorData[params.source] = udata;
			//printf("Ultrasound: %f\n", udata->distance);
		}
	}
}