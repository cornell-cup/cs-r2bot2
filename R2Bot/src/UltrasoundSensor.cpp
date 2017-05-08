#include "Sensor/UltrasoundSensor.h"
#include "Data/UltrasoundData.h"
#include <iostream>

UltrasoundSensor::UltrasoundSensor(string port, int baudrate, string port2, int baudrate2) : Sensor("Ultrasound Sensor"), connF(std::make_shared<SerialPort>(port, baudrate)), connB(std::make_shared<SerialPort>(port2, baudrate2)), dataMutex() {
	printf("Ultrasound connected to port %s\n", port.c_str());
}

UltrasoundSensor::~UltrasoundSensor() {
}

bool UltrasoundSensor::ping() {
	return connF->isConnected() == 1 && connB->isConnected();
}

void UltrasoundSensor::fillData(SensorData& sensorData) {
	if (connF->isConnected()) {
		char data[256];
		int bytesRead = connF->read(data, 256);
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
			if (params.source == "ULTRASOUND") {
				std::vector<float> u;
				string t(params.data.begin(), params.data.end());
				t = t + ",";
				string delimiter = ",";
				size_t pos = 0;
				std::string token;
				int ctr = 0;
				while ((pos = t.find(delimiter)) != std::string::npos) {
					token = t.substr(0, pos);
					//std::cout << token << "\t" << stof(token) << std::endl;
					u.push_back(stof(token));
					t.erase(0, pos + delimiter.length());
				}
				udata->distance = u;

				std::vector<uint8_t> newinput(input.begin() + read, input.end());
				newinput.swap(input);
				if (params.id == "F") {
					sensorData[params.source + "F"] = udata;
				}
				else if (params.id == "B") {
					sensorData[params.source + "B"] = udata;
				}
			}
		}
	}
	if (connB->isConnected()) {
		char data[256];
		int bytesRead = connB->read(data, 256);
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
			if (params.source == "ULTRASOUND") {
				std::vector<float> u;
				string t(params.data.begin(), params.data.end());
				t = t + ",";
				string delimiter = ",";
				size_t pos = 0;
				std::string token;
				int ctr = 0;
				while ((pos = t.find(delimiter)) != std::string::npos) {
					token = t.substr(0, pos);
					u.push_back(stof(token));
					t.erase(0, pos + delimiter.length());
				}
				udata->distance = u;

				std::vector<uint8_t> newinput(input.begin() + read, input.end());
				newinput.swap(input);
				if (params.id == "F") {
					sensorData[params.source + "F"] = udata;
				}
				else if (params.id == "B") {
					sensorData[params.source + "B"] = udata;
				}
			}
		}
	}
}