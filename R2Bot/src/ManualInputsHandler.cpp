#include "JobHandler/ManualInputsHandler.h"
#include "R2Protocol.hpp"
#include "SensorData/GamepadSensorData.h"

bool ManualInputsHandler::registered = JobHandler::RegisterJobHandler("manual-inputs", [](string command) {
	return (ptr<JobHandler>) std::make_shared<ManualInputsHandler>();
});

ManualInputsHandler::ManualInputsHandler(): JobHandler() {
}

ManualInputsHandler::~ManualInputsHandler() {
}

void ManualInputsHandler::execute(deque<Job>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs) {
	// Handle gamepad joystick inputs
	auto gamepad = data.find("gamepad");
	if (gamepad != data.end()) {
		auto gamepaddata = std::dynamic_pointer_cast<GamepadSensorData>(gamepad->second);
		// TODO Turn these into the 12 byte format
		outputs["motor"] = std::to_string(gamepaddata->x) + " " + std::to_string(gamepaddata->y);
	}
}