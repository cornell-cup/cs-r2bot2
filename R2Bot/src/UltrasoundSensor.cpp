#include "Sensor/UltrasoundSensor.h"
#include "Data/ForwardData.h"

UltrasoundSensor::UltrasoundSensor(string port, int baudrate) : Sensor("Ultrasound Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)) {
}

UltrasoundSensor::~UltrasoundSensor() {
}

bool UltrasoundSensor::ping() {
	return conn->isConnected() == 1;
}

void UltrasoundSensor::fillData(smap<ptr<void>>& sensorData) {
	if (conn->isConnected()) {
		char data[256];
		int bytesRead = conn->read(data, 256);

		// Decode the incoming data
		R2Protocol::Packet params;
		std::vector<unsigned char> input(data, data + bytesRead);
		R2Protocol::decode(input, params);
	}
}