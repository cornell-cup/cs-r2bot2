#include "JobHandler/SafetyHandler.h"
#include "R2Protocol.hpp"
#include "Data/MotorData.h"
#include "Data/UltrasoundData.h"

#include <deque>
#include <string>

using std::string;

bool SafetyHandler::registered = JobHandler::RegisterJobHandler("safety", [](string command) {
	return (ptr<JobHandler>) std::make_shared<SafetyHandler>();
});

SafetyHandler::SafetyHandler() : JobHandler() {
}

SafetyHandler::~SafetyHandler() {
}

string SafetyHandler::_pad(int i, unsigned int l) {
	string s = std::to_string(i);
	if (s.size() >= l) {
		return s;
	}
	return s + string(l - s.size(), ' ');
}

void SafetyHandler::execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	// Prevent motors from running if the ultrasound sensors report an object immediately ahead
	auto motor = outputs.find("MOTOR");
	auto ultrasound = data.find("ULTRASOUNDF");
	auto ultrasound2 = data.find("ULTRASOUNDB");
	if (motor != outputs.end()) {
		auto motordata = std::static_pointer_cast<MotorData>(motor->second);
		if (ultrasound == data.end() && ultrasound2 == data.end()) {
			// The motor is not allowed to move without ultrasound safety
			//printf("Motor is not allowed to move without the ultrasound sensor safety\n");
			motordata->leftMotor = 0;
			motordata->rightMotor = 0;
		}
		else {
			auto ultrasounddata = std::static_pointer_cast<UltrasoundData>(ultrasound->second);
			for (int i = 0; i < 14; i++) {
				if (ultrasounddata->distance[i] < 12.0f && (motordata->leftMotor > 0 || motordata->rightMotor > 0)) {
					motordata->leftMotor = 0;
					motordata->rightMotor = 0;
				}
			}
		}
	}
}