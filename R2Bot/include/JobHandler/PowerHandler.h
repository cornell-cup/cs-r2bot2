#ifndef _R2BOT_POWER_HANDLER
#define _R2BOT_POWER_HANDLER

#include "Global.h"
#include "R2Protocol.hpp"
#include "JobHandler.h"
#include "SerialPort.h"

#include <deque>
#include <mutex>
#include <string>

class PowerHandler : public Sensor, public JobHandler{
private:
	static bool registered;
protected:
	ptr<SerialPort> conn;
	std::mutex dataMutex;
public:
	PowerHandler(std::string port, int baudrate);
	~PowerHandler();

	std::string getName();
	bool ping();

	void fillData(SensorData& sensorData);
	void sendData(ControllerData& controllerData);
	void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif