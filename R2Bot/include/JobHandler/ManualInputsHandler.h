#ifndef _R2BOT_MANUAL_INPUTS_HANDLER
#define _R2BOT_MANUAL_INPUTS_HANDLER

#include "Global.h"
#include "JobHandler.h"

#include <chrono>

class ManualInputsHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;

	/** Last input activity */
	std::chrono::time_point<std::chrono::steady_clock> lastActivity;
protected:
public:
	ManualInputsHandler();
	virtual ~ManualInputsHandler();

	virtual void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif