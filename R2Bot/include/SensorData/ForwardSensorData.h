#ifndef _R2BOT_FORWARD_SENSOR_DATA
#define _R2BOT_FORWARD_SENSOR_DATA

#include "Global.h"
#include "R2Protocol.hpp"
#include "SensorData.h"

class ForwardSensorData : public SensorData {
protected:

public:
	/* Create an empty set of sensor data to forward */
	ForwardSensorData();
	/* Create an initial set of sensor data to forward */
	ForwardSensorData(smap<R2Protocol::Packet> data);
	~ForwardSensorData();

	/* Data to be forwarded */
	smap<R2Protocol::Packet> forwardData;
};

#endif