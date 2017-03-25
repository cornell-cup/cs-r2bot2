#include "Controller/MotorController.h"

MotorController::MotorController(string port, int baudrate) : Controller("Motor Controller"), conn(std::make_shared<SerialPort>(port, baudrate)) {
}

MotorController::~MotorController() {
}

bool MotorController::ping() {
	return conn->isConnected();
}

void MotorController::sendData(string data) {
	if (conn->isConnected()) {
		conn->write((char *) data.data(), data.size());
	}
}