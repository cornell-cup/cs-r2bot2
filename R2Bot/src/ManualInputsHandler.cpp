#include "JobHandler/ManualInputsHandler.h"
#include "R2Protocol.hpp"
#include "SensorData/GamepadSensorData.h"

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
	return s + string(l - s.size(), ' ');
}

void ManualInputsHandler::execute(deque<Job>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs) {
	// Handle gamepad joystick inputs
	auto gamepad = data.find("gamepad");
	if (gamepad != data.end()) {
		auto gamepaddata = std::dynamic_pointer_cast<GamepadSensorData>(gamepad->second);
		// Compute tank drive voltages
		float radius = std::sqrt(std::pow(gamepaddata->x, 2) + std::pow(gamepaddata->y, 2));
		float angle = std::atan2(gamepaddata->y, gamepaddata->x);
		// Implement radius deadzone and scale to max speed (200)
		radius = std::fmax(0.f, radius - 0.2f) / 0.8f * 200.f;
		// Implement tank drive by rotating angle by 45 degrees clockwise
		angle = angle - M_PI / 4.f;
		int l = radius * std::cos(angle);
		int r = radius * std::sin(angle);
		string payload = string("M1") + _pad(l, 4) + string("M2") + _pad(r, 4);

		// Pack values into the protocol
		outputs["motor"] = payload;
		/*
		R2Protocol::Packet params;
		params.source = DEVICE_NAME;
		params.destination = "MOTOR";
		params.id = "0";
		params.data = std::vector<uint8_t>(payload.begin(), payload.end());
		vector<uint8_t> output;
		R2Protocol::encode(params, output);
		outputs["motor"] = string(output.begin(), output.end());
		printf("%s\n", payload.c_str());
		for (int i = 0; i < outputs["motor"].size(); i++) {
			printf("%02x", (unsigned char) outputs["motor"][i]);
		}
		printf("\n");
		*/
	}
}