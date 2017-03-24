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
	if (decoder == SensorData::DataDecoders.end()) {
		return 0;
	}
	else {
		return (*decoder).second(data);
	}
}

SensorData::SensorData() {
}

SensorData::~SensorData() {
}