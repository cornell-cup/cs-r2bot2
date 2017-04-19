#include "Sensor/UDPServerSensor.h"
#include "Data/ForwardData.h"


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
			dataReceived[params.source] = params.data;
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

void UDPServerSensor::getData(smap<vector<uint8_t>>& sensorData) {
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
		ForwardData data = { dataToForward };
		auto ptr = (uint8_t*) &data;
		sensorData["FORWARD"] = vector<uint8_t>(ptr, ptr + sizeof(ForwardData));
	}
	else {
		ForwardData* data = (ForwardData*) forward->second.data();
		data->data.insert(dataToForward.begin(), dataToForward.end());
	}
	// Clear the local data
	dataToForward.clear();
}