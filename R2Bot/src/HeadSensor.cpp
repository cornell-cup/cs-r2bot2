#include "Sensor/HeadSensor.h"
#include "Data/HeadData.h"
#include <iostream>

HeadSensor::HeadSensor(string port, int baudrate) : Sensor("Head Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("Head connected to port %s\n", port.c_str());
}

HeadSensor::~HeadSensor() {
}

bool HeadSensor::ping() {
	return conn->isConnected() == 1;
}

void HeadSensor::fillData(SensorData& sensorData) {
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
		ptr<HeadData> hdata = std::make_shared<HeadData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if (params.source == "HEAD") {
				hdata->angle = std::atof((char *)params.data.data());
			}
			hdata->angle = std::atof((char *)params.data.data());
			std::vector<uint8_t> newinput(input.begin() + read, input.end());
			newinput.swap(input);
			sensorData["HEAD"] = hdata;
			printf("Head Angle: %d\n", hdata->angle);
		}
	}
}

void HeadSensor::sendData(ControllerData& data) {
	if (conn->isConnected()) {
		auto result = data.find("HEAD");
		auto headData = result->second;
		if (result != data.end()) {
			ptr<string> type = std::static_pointer_cast<string>(headData);
			string command = *type;
			R2Protocol::Packet params = { DEVICE_NAME, "HEAD", "", vector<uint8_t>(command.begin(), command.end()) };
			vector<uint8_t> output;
			R2Protocol::encode(params, output);
			printf("Head: %s\n", command.c_str());
			conn->write((char *)output.data(), (unsigned int)output.size());
		}
	}
}