#include "JobHandler/ManualInputsHandler.h"
#include "Data/GamepadData.h"
#include "Data/MotorData.h"

#include <cmath>
#define M_PI 3.14159265358979323846

bool ManualInputsHandler::registered = JobHandler::RegisterJobHandler("manual-inputs", [](string command) {
	return (ptr<JobHandler>) std::make_shared<ManualInputsHandler>();
});

ManualInputsHandler::ManualInputsHandler(): JobHandler() {
}

ManualInputsHandler::~ManualInputsHandler() {
}

void ManualInputsHandler::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	// Handle gamepad joystick inputs
	auto gamepad = data.find("GAMEPAD");
	if (gamepad != data.end()) {
		auto gamepaddata = std::static_pointer_cast<GamepadData>(gamepad->second);
		// Compute tank drive voltages
		float radius = std::sqrt(std::pow(gamepaddata->x, 2) + std::pow(gamepaddata->y, 2));
		float angle = std::atan2(gamepaddata->y, gamepaddata->x);
		// Implement radius deadzone and scale to max speed (200)
		radius = std::fmax(0.f, radius - 0.2f) / 0.8f * 200.f;
		// Implement tank drive by rotating angle by 45 degrees clockwise
		angle = angle - (float) M_PI / 4.f;
		int l = (int) (radius * std::cos(angle));
		int r = (int) (radius * std::sin(angle));
		ptr<MotorData> output = std::make_shared<MotorData>();
		output->leftMotor = l;
		output->rightMotor = r;
		outputs["MOTOR"] = output;
		printf("%d %d\n", output->leftMotor, output->rightMotor);
	}
	else {
		ptr<MotorData> output = std::make_shared<MotorData>();
		output->leftMotor = 0;
		output->rightMotor = 0;
		outputs["MOTOR"] = output;
		printf("%d %d\n", output->leftMotor, output->rightMotor);
	}
}