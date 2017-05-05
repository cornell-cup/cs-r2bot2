#include "Sensor/DrawerSensor.h"
#include "Data/DrawerData.h"
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