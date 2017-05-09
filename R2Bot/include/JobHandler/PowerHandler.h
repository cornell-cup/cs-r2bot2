#ifndef _R2BOT_POWER_HANDLER
#define _R2BOT_POWER_HANDLER

#include "Global.h"
#include "R2Protocol.hpp"
#include "JobHandler.h"
#include "SerialPort.h"

#include <mutex>

class PowerHandler : public Sensor, public JobHandler{
private:
	static bool registered;
protected:
	std::mutex dataMutex;
	ptr<SerialPort> conn;
public:
	PowerHandler(string port, int baudrate);
	~PowerHandler();

	std::string getName();
	bool ping();

	void fillData(SensorData& sensorData);
	void sendData(ControllerData& controllerData);
	void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif