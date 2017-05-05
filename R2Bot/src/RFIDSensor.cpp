#include "Sensor/RFIDSensor.h"
#include "Sensor/DrawerSensor.h"
#include "Data/RFIDData.h"
#include "Data/RFIDCommand.h"
#include <iostream>

RFIDSensor::RFIDSensor(string port, int baudrate) : Sensor("RFID Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("RFID connected to port %s\n", port.c_str());
}

RFIDSensor::~RFIDSensor() {
}

bool RFIDSensor::ping() {
	return conn->isConnected() == 1;
}

void RFIDSensor::fillData(SensorData& sensorData) {
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
		ptr<RFIDData> rdata = std::make_shared<RFIDData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if (params.source.c_str() == "RFID") {
				rdata->ID = std::atof((char *)params.data.data());
			}
			rdata->ID = std::atof((char *)params.data.data());
			std::vector<uint8_t> newinput(input.begin() + read, input.end());
			newinput.swap(input);
			sensorData["RFID"] = rdata;
			printf("RFID: %f\n", rdata->ID);
		}
	}
}

void RFIDSensor::sendData(ControllerData& data) {
	if (conn->isConnected()) {
		auto result = data.find("DATABASE");
		ptr<RFIDCommand> cdata = std::make_shared<RFIDCommand>();
		bool drawerState;
		if (result != data.end() && !drawerState) {
			if (result->second) {
				drawerState = true;
				string command = "O";
				data["RFIDCOMMAND"] = std::shared_ptr<string>("O");
				cdata->state = "O";
				R2Protocol::Packet params = { DEVICE_NAME, "DRAWER1", "", vector<uint8_t>(command.begin(), command.end()) };
				vector<uint8_t> output;
				R2Protocol::encode(params, output);
				printf("User authenticed. Open drawer.\n");
				conn->write((char *)output.data(), (unsigned int)output.size());
			}
		}
		else if (result != data.end() && drawerState) {
			if (result->second) {
				//closing the drawer
				drawerState = false;
				string command = "C";
				cdata->state = "C";
				data["RFIDCOMMAND"] = std::shared_ptr<string>("C");
				R2Protocol::Packet params = { DEVICE_NAME, "DRAWER1", "", vector<uint8_t>(command.begin(), command.end()) };
				vector<uint8_t> output;
				R2Protocol::encode(params, output);
				printf("User authenticed. Close drawer.\n");
				conn->write((char *)output.data(), (unsigned int)output.size());


				//immediately getting inventory after closing drawer
				command = "T";
				data["RFIDCOMMAND"] = std::shared_ptr<string>("T");
				cdata->state = "T";
				R2Protocol::Packet params2 = { DEVICE_NAME, "DRAWER1", "", vector<uint8_t>(command.begin(), command.end()) };
				vector<uint8_t> output2;
				R2Protocol::encode(params2, output2);
				printf("User authenticed. Close drawer.\n");
				conn->write((char *)output.data(), (unsigned int)output2.size());

			}
		}
	}
}