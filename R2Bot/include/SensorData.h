#ifndef _R2BOT_SENSORDATA
#define _R2BOT_SENSORDATA

#include "Global.h"

#include <string>

class SensorData;

typedef vector<uint8_t> (*SensorDataEncoder)(ptr<SensorData>);
typedef ptr<SensorData> (*SensorDataDecoder)(vector<uint8_t>);

class SensorData {
private:
	/** Register this sensor data */
	static bool registered;
protected:
	/** Types of sensor data and pointers to encoding and decoding functions */
	static smap<SensorDataEncoder>& DataEncoders();
	static smap<SensorDataDecoder>& DataDecoders();

	/** Names of the sensors this data may correspond to */
	static string names;

public:
	/** Register this sensor data for discovering arbitrary data */
	static bool RegisterSensorData(string dataName, SensorDataEncoder encoderFunc, SensorDataDecoder decoderFunc);

	/** Decode arbitrary data */
	static ptr<SensorData> DecodeSensorData(string dataType, vector<uint8_t> data);

	SensorData();
	virtual ~SensorData();
};

#endif
