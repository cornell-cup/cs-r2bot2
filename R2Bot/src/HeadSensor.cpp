#include "Sensor/HeadSensor.h"
#include "Data/HeadData.h"
#include <iostream>

HeadSensor::HeadSensor(string port, int baudrate) : Controller("Head Controller"), Sensor("Head Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
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
				hdata->angle = static_cast<int>(std::atof((char *)params.data.data()));
			}
			hdata->angle = static_cast<int>(std::atof((char *)params.data.data()));
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
			string command;
			ptr<HeadData> type = std::static_pointer_cast<HeadData>(headData);
			char commandType = type->command;
			if (commandType == 'L' || commandType == 'R'){
				if (type->time != 0) {
					command = commandType + std::to_string(type->time);
				}
			}
			else if (commandType == 'G') {
				command = commandType;
			}
			else if (commandType == 'P') {
				if (type->angle != 0) {
					command = commandType + std::to_string(type->angle);
				}
			}
			R2Protocol::Packet params = { R2Bot::DEVICE_NAME, "HEAD", "", vector<uint8_t>(command.begin(), command.end()) };
			vector<uint8_t> output;
			R2Protocol::encode(params, output);
			printf("Head: %s\n", command.c_str());
			conn->write((char *)output.data(), (unsigned int)output.size());
		}
	}
}