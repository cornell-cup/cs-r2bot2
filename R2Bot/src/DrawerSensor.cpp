#include "Sensor/DrawerSensor.h"
#include "Data/ForwardData.h"
#include "Controller/MotorController.h"
#include <vector>

DrawerSensor::DrawerSensor(string port, int baudrate) : Sensor("Drawer Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)),
dataMutex(), dataReceived(), dataToForward() {
		// Decode the incoming data
		R2Protocol::Packet params;
		char data[256];
		int bytesRead = conn->read(data, 256);
		std::vector<unsigned char> input(data, data + bytesRead);
		R2Protocol::decode(input, params);

		std::lock_guard<std::mutex> lock(dataMutex);
		if (params.destination == DEVICE_NAME) {
			// Data is sent here
			ptr<void> data(malloc(params.data.size()), free);
			std::memcpy(data.get(), params.data.data(), params.data.size());
			dataReceived[params.source] = data;
		}
		else {
			// Data should be forwarded
			dataToForward[params.destination] = params;
		}
}

DrawerSensor::~DrawerSensor() {
}

bool DrawerSensor::ping() {
	return conn->isConnected() == 1;
}

void DrawerSensor::fillData(SensorData & sensorData) {
	std::lock_guard<std::mutex> lock(dataMutex);
	for (auto itr : dataReceived) {
		// Copy data to the sensor data
		sensorData[itr.first] = itr.second;
	}
	// Clear the local data
	dataReceived.clear();

	// Copy forwarded data
	auto forward = sensorData.find("FORWARD");
	if (forward == sensorData.end()) {
		ptr<ForwardData> data = std::make_shared<ForwardData>();
		data->data = dataToForward;
		sensorData["FORWARD"] = data;
	}
	else {
		ptr<ForwardData> data = std::static_pointer_cast<ForwardData>(forward->second);
		data->data.insert(dataToForward.begin(), dataToForward.end());
	}
	// Clear the local data
	dataToForward.clear();
}

void DrawerSensor::sendData(ControllerData & data) {
	if (conn->isConnected()) {
		auto result = data.find("FORWARD");
		if (result != data.end()) {
			ptr<ForwardData> c = std::static_pointer_cast<ForwardData>(result->second);
			for (auto s : c->data) {
				std::vector<uint8_t> output;
				R2Protocol::encode(s.second, output);
				conn->write((char *)output.data(), (unsigned int)output.size());
			}
		}
	}

}
