#include "Controller/MotorController.h"
#include "Data/MotorData.h"
#include "R2Protocol.hpp"

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

using std::min;
using std::max;
using std::string;
using std::vector;

MotorController::MotorController(string port, int baudrate) : Controller("Motor Controller"), conn(std::make_shared<SerialPort>(port, baudrate)) {
	printf("Motor connected to port %s\n", port.c_str());
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
			// Cap speeds at the defined maximum
			int leftMotor = max(min(m->leftMotor, MOTOR_MAX_SPEED), -MOTOR_MAX_SPEED);
			int rightMotor = max(min(m->rightMotor, MOTOR_MAX_SPEED), -MOTOR_MAX_SPEED);
			// Pack values into 12 bytes
			string command = string("M1") + _pad(leftMotor, 4) + string("M2") + _pad(rightMotor, 4);
			R2Protocol::Packet params = { R2Bot::DEVICE_NAME, "MOTOR", "", vector<uint8_t>(command.begin(), command.end()) };
			vector<uint8_t> output;
			R2Protocol::encode(params, output);
			printf("Motors: %d %d\n", m->leftMotor, m->rightMotor);
			conn->write((char *) output.data(), (unsigned int)output.size());
		}
	}
}