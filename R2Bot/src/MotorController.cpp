#include "Controller/MotorController.h"
#include "Data/MotorData.h"
#include "R2Protocol.hpp"

#include <cstring>

MotorController::MotorController(string port, int baudrate) : Controller("Motor Controller"), conn(std::make_shared<SerialPort>(port, baudrate)) {
}

MotorController::~MotorController() {
}

string MotorController::_pad(int i, unsigned int l) {
	string s = std::to_string(i);
	if (s.size() >= l) {
		return s;
	}
	return s + string(l - s.size(), ' ');
}

bool MotorController::ping() {
	return conn->isConnected() == 1;
}

void MotorController::sendData(ControllerData& data) {
	if (conn->isConnected()) {
		auto result = data.find("MOTOR");
		if (result != data.end()) {
			ptr<MotorData> m = std::static_pointer_cast<MotorData>(result->second);
			// Pack values into 12 bytes
			string command = string("M1") + _pad(m->leftMotor, 4) + string("M2") + _pad(m->rightMotor, 4);
			// Protocol disabled because decoding is not working on the PIC32
			//R2Protocol::Packet params = { DEVICE_NAME, "MOTOR", "", vector<uint8_t>(command.begin(), command.end()) };
			//vector<uint8_t> output;
			//R2Protocol::encode(params, output);
			printf("Motors: %d %d\n", m->leftMotor, m->rightMotor);
			char *c = new char[command.length() + 1];
			std::strcpy(c, command.c_str());
			conn->write(c, (unsigned int)command.size());
		}
	}
}