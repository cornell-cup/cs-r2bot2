#ifndef _R2BOT_HEAD_HANDLER
#define _R2BOT_HEAD_HANDLER

#include "Global.h"
#include "JobHandler.h"

#include <deque>

class HeadHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;

public:
	HeadHandler();
	virtual ~HeadHandler();

	virtual void execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif