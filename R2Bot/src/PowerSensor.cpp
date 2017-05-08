#include "Sensor/PowerSensor.h"
#include "Data/PowerData.h"
#include <iostream>

PowerSensor::PowerSensor(string port, int baudrate) : Sensor("Power Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("Power sensor connected to port %s\n", port.c_str());
}

PowerSensor::~PowerSensor() {
}

bool PowerSensor::ping() {
	return conn->isConnected() == 1;
}

void PowerSensor::fillData(SensorData& sensorData) {
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
		ptr<PowerData> pdata = std::make_shared<PowerData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if (params.source == "POWER") {
				string powerData = (char *)params.data.data();
				string delimiter = "|";
				size_t pos = 0;
				std::string token;
				int ctr = 0;
				while ((pos = powerData.find(delimiter)) != std::string::npos) {
					token = powerData.substr(2, pos);
					if (ctr == 0) {
						pdata->voltage = stof(token);
						ctr++;
					}
					else if (ctr == 1) {
						pdata->current = stof(token);
						ctr++;
					}
					else if (ctr == 2) {
						pdata->batteryCapacity = stof(token);
						ctr++;
					}
					else if (ctr == 3) {
						pdata->batteryLevel = stof(token);
						ctr = 0;
					}
					powerData.erase(0, pos + delimiter.length());
				}
				
			}
			std::vector<uint8_t> newinput(input.begin() + read, input.end());
			newinput.swap(input);
			sensorData["POWER"] = pdata;
			printf("Voltage: %f \t Current: %f \t Battery Capacity: %f \t Battery Level: %f \n", pdata->voltage, pdata->current, pdata->batteryCapacity, pdata->batteryLevel);
		}
	}
}

void PowerSensor::sendData(ControllerData& data) {
	if (conn->isConnected()) {
		auto result = data.find("POWER");
		auto powerQuery = result->second;
		if (result != data.end()) {
			ptr<string> type = std::static_pointer_cast<string>(powerQuery);
			string command = *type;
			R2Protocol::Packet params = { DEVICE_NAME, "POWER", "", vector<uint8_t>(command.begin(), command.end()) };
			vector<uint8_t> output;
			R2Protocol::encode(params, output);
			printf("Power: %s\n", command.c_str());
			conn->write((char *)output.data(), (unsigned int)output.size());
		}
	}
}