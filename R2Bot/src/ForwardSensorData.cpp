#include "SensorData/ForwardSensorData.h"

bool ForwardSensorData::registered = SensorData::RegisterSensorData("forward", [](ptr<SensorData> command) { return vector<uint8_t>(); },
	[](vector<uint8_t> data) { return (ptr<SensorData>) std::make_shared<SensorData>(); });

ForwardSensorData::ForwardSensorData(): SensorData(), forwardData() {
}

ForwardSensorData::ForwardSensorData(smap<R2Protocol::Packet> data): SensorData(), forwardData(data) {
}

ForwardSensorData::~ForwardSensorData() {
}