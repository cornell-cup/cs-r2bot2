#include "Sensor/UDPServerSensor.h"
#include "SensorData/ForwardSensorData.h"


UDPServerSensor::UDPServerSensor(string host, int port) : Sensor("UDP Server"), server(std::make_shared<UDPSocketServer>(host, port)),
dataMutex(), dataReceived(), dataToForward() {
	server->server([this](char * buffer, unsigned int buffer_len) {
		// Decode the incoming data
		R2Protocol::Packet params;
		std::vector<unsigned char> input(buffer, buffer + buffer_len);
		R2Protocol::decode(input, params);

		std::lock_guard<std::mutex> lock(dataMutex);
		if (params.destination == DEVICE_NAME) {
			// Data is sent here
			dataReceived[params.source] = SensorData::DecodeSensorData(params.source, params.data);
		}
		else {
			// Data should be forwarded
			dataToForward[params.destination] = params;
		}
	});
}
UDPServerSensor::~UDPServerSensor() {
	server->close();
}

bool UDPServerSensor::ping() {
	return server->isListening() == 1;
}

void UDPServerSensor::getData(smap<ptr<SensorData>>& sensorData) {
	std::lock_guard<std::mutex> lock(dataMutex);
	for (auto itr : dataReceived) {
		// Copy data to the sensor data
		sensorData[itr.first] = itr.second;
	}
	// Clear the local data
	dataReceived.clear();

	// Copy forwarded data
	auto forward = sensorData.find("forward");
	if (forward == sensorData.end()) {
		sensorData["forward"] = std::make_shared<ForwardSensorData>(dataToForward);
	}
	else {
		auto data = std::dynamic_pointer_cast<ForwardSensorData>(forward->second);
		data->forwardData.insert(dataToForward.begin(), dataToForward.end());
	}
	// Clear the local data
	dataToForward.clear();
}