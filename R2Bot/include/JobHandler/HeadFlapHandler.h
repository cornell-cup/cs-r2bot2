#ifndef _R2BOT_HEAD_FLAP_HANDLER
#define _R2BOT_HEAD_FLAP_HANDLER

#include "Global.h"
#include "JobHandler.h"

class HeadFlapHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;

public:
	HeadFlapHandler();
	virtual ~HeadFlapHandler();

	virtual void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif