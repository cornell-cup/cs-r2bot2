#include "JobHandler/PowerHandler.h"
#include "Data/PowerData.h"
#include "Data/MotorData.h"
#include <iostream>
#ifdef _WIN32
	#include "Windows.h"
	#include "Winreg.h"
#endif

#include <deque>
#include <string>
#include <vector>

using std::string;
using std::vector;

PowerHandler::PowerHandler(string port, int baudrate) : Sensor("Power Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("Power sensor connected to port %s\n", port.c_str());
}

PowerHandler::~PowerHandler() {
}

string PowerHandler::getName() {
	return "R2 Server";
}

bool PowerHandler::ping() {
	return conn->isConnected() == 1;
}

void PowerHandler::fillData(SensorData& sensorData) {
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
				powerData = powerData + "|";
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

void PowerHandler::sendData(ControllerData& data) {
	if (conn->isConnected()) {
		auto result = data.find("POWER");
		auto powerData = result->second;
		if (result != data.end()) {
			ptr<string> type = std::static_pointer_cast<string>(powerData);
			string command = *type;
			R2Protocol::Packet params = { R2Bot::DEVICE_NAME, "POWER", "", vector<uint8_t>(command.begin(), command.end()) };
			vector<uint8_t> output;
			R2Protocol::encode(params, output);
			printf("POWER: %s\n", command.c_str());
			conn->write((char *)output.data(), (unsigned int)output.size());
		}
	}
}

void PowerHandler::execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	if (conn->isConnected()) {
		auto result = data.find("POWER");
		auto motor = outputs.find("MOTOR");
		if (result != data.end() && motor != data.end()) {
			float v = std::static_pointer_cast<PowerData>(result->second)->voltage;
			auto motordata = std::static_pointer_cast<MotorData>(motor->second);
			if (v <= 25 && v > 24) {
				outputs["SOUND"] = std::make_shared<string>("panic.wav");
			}
			if (v <= 24) {
				motordata->leftMotor = 0;
				motordata->rightMotor = 0;
#ifdef _WIN32
				InitiateSystemShutdown(NULL, NULL, 5, TRUE, FALSE);
#endif	
			}
		}
	}
}