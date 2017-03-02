#ifndef _R2BOT_SENSOR
#define _R2BOT_SENSOR

#include "Global.h"

class Sensor
{
protected:
	/** Unique name of the sensor */
	std::string name;

	/** Data from the sensor */
	void *data;

public:
	Sensor(std::string name);
	Sensor();
	virtual ~Sensor();

	/** Return the name of the sensor */
	std::string getName();

	/** Return true if communication to the sensor is active */
	virtual bool ping();

	/** Add data from the sensor */
	virtual void getData(smap<void*> sensorData);
};

#endif