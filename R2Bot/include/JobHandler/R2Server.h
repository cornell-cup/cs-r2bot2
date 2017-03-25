#ifndef _R2BOT_R2SERVER
#define _R2BOT_R2SERVER

#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif

#include "JobHandler.h"
#include "Sensor.h"

#include "crow_all.h"

class R2Server : public Sensor, public JobHandler {
public
	R2Server(string host, int port);
	virtual ~R2Server();

	/** Return the name of the sensor */
	std::string getName();

	/** Return true if communication to the sensor is active */
	virtual bool ping();

	/** Add data from the sensor */
	virtual void getData(smap<ptr<SensorData>>& sensorData);
};

#endif