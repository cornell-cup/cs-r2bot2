#include "Sensor/RFIDSensor.h"
#include "Data/RFIDData.h"
#include <iostream>

RFIDSensor::RFIDSensor(string port, int baudrate) : Sensor("RFID Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)) {
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

		// Decode the incoming data
		R2Protocol::Packet params;
		std::vector<uint8_t> input(data, data + bytesRead);
		int32_t read;
		ptr<RFIDData> rdata = std::make_shared<RFIDData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if (params.source.c_str() == "HEAD") {
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
		auto result = data.find("RFID");
		if (result != data.end()) {
			ptr<RFIDData> r = std::static_pointer_cast<RFIDData>(result->second);
			// Pack values into 12 bytes
			string command;
			R2Protocol::Packet params = { DEVICE_NAME, "MOTOR", "", vector<uint8_t>(command.begin(), command.end()) };
			vector<uint8_t> output;
			R2Protocol::encode(params, output);
			printf("RFID: %d\n", r->ID);
			conn->write((char *)output.data(), (unsigned int)output.size());
		}
	}
}