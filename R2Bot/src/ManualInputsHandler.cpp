#include "JobHandler/ManualInputsHandler.h"
#include "R2Protocol.hpp"
#include "SensorData/GamepadSensorData.h"

ManualInputsHandler::ManualInputsHandler() {
}

ManualInputsHandler::~ManualInputsHandler() {
}

void ManualInputsHandler::execute(smap<ptr<Job>>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs) {
	// Handle gamepad joystick inputs
	auto gamepad = data.find("gamepad");
	if (gamepad != data.end()) {
		auto gamepaddata = std::dynamic_pointer_cast<GamepadSensorData>(gamepad->second);
		outputs["motor"] = std::to_string(gamepaddata->x) + " " + std::to_string(gamepaddata->y);
		printf("%s\n", outputs["motor"]);
	}
}