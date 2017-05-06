#include "Controller/HeadFlapController.h"
#include "R2Protocol.hpp"

#include <cstring>

HeadFlapController::HeadFlapController(string port, int baudrate) : Controller("Head Flap Controller"), conn(std::make_shared<SerialPort>(port, baudrate)) {
	printf("Head Flap connected to port %s\n", port.c_str());
}

HeadFlapController::~HeadFlapController() {
}

bool HeadFlapController::ping() {
	return conn->isConnected() == 1;
}

void HeadFlapController::sendData(ControllerData& data) {
	if (conn->isConnected()) {
		auto result = data.find("FLAP");
		if (result != data.end()) {
			auto flapData = result->second;
			ptr<string> fdata = std::static_pointer_cast<string>(flapData);
			string command = *fdata;
			R2Protocol::Packet params = { DEVICE_NAME, "FLAP", "", vector<uint8_t>(command.begin(), command.end()) };
			vector<uint8_t> output;
			R2Protocol::encode(params, output);
			printf("Flap: %s\n", command == "C" ? "Close" : "Open");
			conn->write((char *)output.data(), (unsigned int)output.size());
		}
	}
}