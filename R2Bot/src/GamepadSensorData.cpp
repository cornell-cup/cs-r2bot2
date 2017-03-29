#include "SensorData/GamepadSensorData.h"

bool GamepadSensorData::registered = SensorData::RegisterSensorData("gamepad",
	[](ptr<SensorData> data) {
	auto gamepaddata = std::dynamic_pointer_cast<GamepadSensorData>(data);
	string output(std::to_string(gamepaddata->x) + " " + std::to_string(gamepaddata->y));
	return vector<uint8_t>(output.begin(), output.end());
},
[](vector<uint8_t> data) {
	float x, y;
	if (!data.empty()) {
#ifdef _WIN32
		sscanf_s((char *)data.data(), "%f %f", &x, &y);
#else
		sscanf((char *)data.data(), "%f %f", &x, &y);
#endif
	}
	return (ptr<SensorData>) std::make_shared<GamepadSensorData>(x, y);
});

GamepadSensorData::GamepadSensorData(float x, float y) : SensorData(), x(x), y(y) {
}

GamepadSensorData::~GamepadSensorData() {
}