#include "Sensor/DrawerSensor.h"
#include "Data/ForwardData.h"

DrawerSensor::DrawerSensor(string port, int baudrate) : Sensor("Drawer Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)),
dataMutex(), dataReceived(), dataToForward() {
	std::lock_guard<std::mutex> lock(dataMutex);
	if (conn->isConnected()) {
		char data[256];
		int bytesRead = conn->read(data, 256);

		// Decode the incoming data
		R2Protocol::Packet params;
		std::vector<unsigned char> input(data, data + bytesRead);
		R2Protocol::decode(input, params); 
		if (params.destination == DEVICE_NAME) {
			// Data is sent here
			dataReceived[params.source] = params.data;
		}
		else {
			// Data should be forwarded
			dataToForward[params.destination] = params;
		}
	}
}

DrawerSensor::~DrawerSensor() {
}

bool DrawerSensor::ping() {
	return conn->isConnected() == 1;
}

void DrawerSensor::getData(smap<void*>& sensorData) {
	// TODO 
}