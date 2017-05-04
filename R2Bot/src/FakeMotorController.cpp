#include "Controller/FakeMotorController.h"
#include "Data/MotorData.h"

#include <cstring>

FakeMotorController::FakeMotorController() : Controller("Fake Motor Controller") {
	printf("Fake motor connected\n");
}

FakeMotorController::~FakeMotorController() {
}

bool FakeMotorController::ping() {
	return true;
}

void FakeMotorController::sendData(ControllerData& data) {
	auto result = data.find("MOTOR");
	if (result != data.end()) {
		ptr<MotorData> m = std::static_pointer_cast<MotorData>(result->second);
		printf("Motors: %d %d\n", m->leftMotor, m->rightMotor);
	}
}