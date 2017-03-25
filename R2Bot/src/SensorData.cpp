#include "SensorData.h"

smap<SensorDataEncoder>& SensorData::DataEncoders() {
	static smap<SensorDataEncoder> encoders;
	return encoders;
}

smap<SensorDataDecoder>& SensorData::DataDecoders() {
	static smap<SensorDataDecoder> decoders;
	return decoders;
}

bool SensorData::registered = SensorData::RegisterSensorData("default", [](ptr<SensorData> command) { return vector<uint8_t>(); },
		[](vector<uint8_t> data) { return (ptr<SensorData>) std::make_shared<SensorData>(); });

string SensorData::names = { "" };

bool SensorData::RegisterSensorData(string dataName, SensorDataEncoder encoderFunc, SensorDataDecoder decoderFunc) {
	SensorData::DataEncoders()[dataName] = encoderFunc;
	SensorData::DataDecoders()[dataName] = decoderFunc;
	return true;
}

ptr<SensorData> SensorData::DecodeSensorData(string dataType, vector<uint8_t> data) {
	auto decoder = SensorData::DataDecoders().find(dataType);
	if (decoder != SensorData::DataDecoders().end()) {
		return (*decoder).second(data);
	}
	else {
		return std::make_shared<SensorData>();
	}
}

SensorData::SensorData() {
}

SensorData::~SensorData() {
}