#include "SensorData.h"

smap<SensorDataEncoder> SensorData::DataEncoders;
smap<SensorDataDecoder> SensorData::DataDecoders;
string SensorData::names = { "" };

void SensorData::RegisterSensorData(string dataName, SensorDataEncoder encoderFunc, SensorDataDecoder decoderFunc) {
	SensorData::DataEncoders[dataName] = encoderFunc;
	SensorData::DataDecoders[dataName] = decoderFunc;
}

ptr<SensorData> SensorData::DecodeSensorData(string dataType, vector<uint8_t> data) {
	auto decoder = SensorData::DataDecoders.find(dataType);
	if (decoder != SensorData::DataDecoders.end()) {
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