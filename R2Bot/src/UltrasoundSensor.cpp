#include "Sensor/UltrasoundSensor.h"
#include "Data/UltrasoundData.h"

UltrasoundSensor::UltrasoundSensor(string port, int baudrate) : Sensor("Ultrasound Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)) {
}

UltrasoundSensor::~UltrasoundSensor() {
}

bool UltrasoundSensor::ping() {
	return conn->isConnected() == 1;
}

void UltrasoundSensor::fillData(SensorData& sensorData) {
	if (conn->isConnected()) {
		char data[256];
		int bytesRead = conn->read(data, 256);

		// Decode the incoming data
		R2Protocol::Packet params;
		std::vector<uint8_t> input(data, data + bytesRead);
		int32_t read;
		ptr<UltrasoundData> udata = std::make_shared<UltrasoundData>();
		while ((read = R2Protocol::decode(input, params)) >= 0) {
			if (params.source == "U1SENSOR") {
				udata->distance = std::atof((char *) params.data.data());
			}
			std::vector<uint8_t>(input.begin() + read, input.end()).swap(input);
		}
		sensorData["ULTRASOUND"] = udata;
		printf("Ultrasound: %f\n", udata->distance);
	}
}