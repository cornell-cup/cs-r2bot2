#include "JobHandler/ManualInputsHandler.h"
#include "R2Protocol.hpp"
#include "Data/GamepadData.h"

#include <cmath>
#define M_PI 3.14159265358979323846

bool ManualInputsHandler::registered = JobHandler::RegisterJobHandler("manual-inputs", [](string command) {
	return (ptr<JobHandler>) std::make_shared<ManualInputsHandler>();
});

ManualInputsHandler::ManualInputsHandler(): JobHandler() {
}

ManualInputsHandler::~ManualInputsHandler() {
}

string ManualInputsHandler::_pad(int i, int l) {
	string s = std::to_string(i);
	if (s.size() >= l) {
		return s;
	}
	return s + string(l - s.size(), ' ');
}

void ManualInputsHandler::execute(deque<Job>& jobs, SensorData& data, smap<string>& outputs) {
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
		// Pack values into 12 bytes
		string command = string("M1") + _pad(l, 4) + string("M2") + _pad(r, 4);
		R2Protocol::Packet params = { DEVICE_NAME, "MOTOR", "", vector<uint8_t>(command.begin(), command.end()) };
		vector<uint8_t> output;
		R2Protocol::encode(params, output);
		outputs["motor"] = string(output.begin(), output.end());
		printf("%s\n", command.c_str());
	}
}