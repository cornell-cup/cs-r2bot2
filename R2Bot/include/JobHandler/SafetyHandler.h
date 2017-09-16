#ifndef _R2BOT_SAFETY_HANDLER
#define _R2BOT_SAFETY_HANDLER

#include "Global.h"
#include "JobHandler.h"

#include <deque>
#include <string>

class SafetyHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;

	/** Pad an integer into a string */
	std::string _pad(int i, unsigned int l);
protected:
public:
	SafetyHandler();
	virtual ~SafetyHandler();

	virtual void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif