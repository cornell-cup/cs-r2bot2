#include "SensorData/ForwardSensorData.h"

ForwardSensorData::ForwardSensorData(): SensorData(), forwardData() {
}

ForwardSensorData::ForwardSensorData(smap<R2Protocol::Packet> data): SensorData(), forwardData(data) {
}

ForwardSensorData::~ForwardSensorData() {
}