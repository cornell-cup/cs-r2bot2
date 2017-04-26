#include "Controller/MotorController.h"
#include "Data/ForwardData.h"

MotorController::MotorController(string port, int baudrate) : Controller("Motor Controller"), conn(std::make_shared<SerialPort>(port, baudrate)) {
}

MotorController::~MotorController() {
}

bool MotorController::ping() {
	return conn->isConnected() == 1;
}

void MotorController::sendData(ControllerData & data) {
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