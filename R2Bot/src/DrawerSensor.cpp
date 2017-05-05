#include "Sensor/DrawerSensor.h"
#include "Data/DrawerData.h"
#include "Data/DrawerCommand.h"
#include <iostream>

DrawerSensor::DrawerSensor(string port, int baudrate) : Sensor("Drawer Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("Drawer connected to port %s\n", port.c_str());
}

DrawerSensor::~DrawerSensor() {
}

bool DrawerSensor::ping() {
	return conn->isConnected() == 1;
}

void DrawerSensor::fillData(SensorData& sensorData) {
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
		ptr<DrawerData> ddata = std::make_shared<DrawerData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {	
			ddata->tool0 = params.data.data()[0];
			ddata->tool1 = params.data.data()[1];
			ddata->tool2 = params.data.data()[2];
			ddata->tool3 = params.data.data()[3];
			ddata->tool4 = params.data.data()[4];
			ddata->tool5 = params.data.data()[5];
			std::vector<uint8_t> newinput(input.begin() + read, input.end());
			newinput.swap(input);
			sensorData["DRAWER"] = ddata;
			printf("DRAWER: %f\n", ddata->tool0);
		}
	}
}

void DrawerSensor::sendData(ControllerData& data) {
	if (conn->isConnected()) {
		auto result = data.find("DATABASE");
		ptr<DrawerCommand> cdata = std::make_shared<DrawerCommand>();
		bool drawerState;
		if (result != data.end() && !drawerState) {
			if (result->second) {
				drawerState = true;
				string command = "O";
				cdata->state = command;
				data["DRAWERCOMMAND"] = std::make_shared<string>(command);
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
				cdata->state = command;
				data["DRAWERCOMMAND"] = std::make_shared<string>(command);
				R2Protocol::Packet params = { DEVICE_NAME, "DRAWER1", "", vector<uint8_t>(command.begin(), command.end()) };
				vector<uint8_t> output;
				R2Protocol::encode(params, output);
				printf("User authenticed. Close drawer.\n");
				conn->write((char *)output.data(), (unsigned int)output.size());


				//immediately getting inventory after closing drawer
				command = "T";
				cdata->state = command;
				data["DRAWERCOMMAND"] = std::make_shared<string>(command);
				R2Protocol::Packet params2 = { DEVICE_NAME, "DRAWER1", "", vector<uint8_t>(command.begin(), command.end()) };
				vector<uint8_t> output2;
				R2Protocol::encode(params2, output2);
				printf("Tool inventory updated.\n");
				conn->write((char *)output.data(), (unsigned int)output2.size());

			}
		}
	}
}