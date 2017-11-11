#include "Controller/MotorController.h"
#include "Data/MotorData.h"
#include "Data/HeadData.h"
#include "R2Protocol.hpp"

#include <algorithm>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>

using std::min;
using std::max;
using std::string;
using std::vector;

MotorController::MotorController(string port, int baudrate) : Controller("Motor Controller"), conn(std::make_shared<SerialPort>(port, baudrate)), counter(0) {
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
            char command[8] = {
                '\n',
                (leftMotor < 0) ? 'l' : 'L',
                (char) (std::abs(leftMotor) / 2),
                '\n',
                (rightMotor < 0) ? 'r' : 'R',
                (char) (std::abs(rightMotor) / 2),
                '\n',
                '\0'
            };
			//R2Protocol::Packet params = { R2Bot::DEVICE_NAME, "MOTOR", "", vector<uint8_t>(command.begin(), command.end()) };
			//vector<uint8_t> output;
			//R2Protocol::encode(params, output);
			printf("Motors: %d %d\n", m->leftMotor, m->rightMotor);
			conn->write(command, 8);
		}
        result = data.find("HEAD");
        if (result != data.end()) {
            ptr<HeadData> h = std::static_pointer_cast<HeadData>(result->second);
            int value = 90;
            if (h->command == 'L') {
                value = 60;
            }
            else if (h->command == 'R') {
                value = 120;
            }
            string command = string("\nH") + ((char)(value / 2)) + string("\n");
            conn->write((char *) command.data(), (unsigned int)command.size());
        }
	}
    counter++;
}
