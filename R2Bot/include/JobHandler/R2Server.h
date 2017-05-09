#ifndef _R2BOT_R2SERVER
#define _R2BOT_R2SERVER

#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif

#include "Global.h"
#include "JobHandler.h"
#include "Sensor.h"

#include "crow_all.h"

#include <unordered_set>

class R2Server : public Sensor, public JobHandler {
private:
	static bool registered;
protected:
	crow::SimpleApp app;
	std::mutex mtx;
	std::unordered_set<crow::websocket::connection*> users;

public:
	R2Server(int port);
	virtual ~R2Server();

	/** Return the name of the sensor */
	std::string getName();

	/** Return true if communication to the sensor is active */
	virtual bool ping();

	/** Add data from the sensor */
	virtual void fillData(SensorData& sensorData);

	/** Runs the job handler's actions */
	virtual void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif