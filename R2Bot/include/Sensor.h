#ifndef _Sensor
#define _Sensor

#include <string>

class Sensor
{
protected:
	/** Unique name of the sensor */
	std::string name;

	/** Data from the sensor */
	void *data;

public:
	Sensor(std::string name);
	virtual ~Sensor();

	/** Return the name of the sensor. */
	std::string getName();

	/** Return true if communication to the sensor is active. */
	virtual bool ping();

	/** Return the last retrieved data from the sensor. */
	virtual void *getCachedData();

	/** Retrieve and store data from the sensor. Return true if successful. */
	virtual bool getSensorData();
};

#endif
