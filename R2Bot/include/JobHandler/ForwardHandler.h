#ifndef _R2BOT_FORWARD_HANDLER
#define _R2BOT_FORWARD_HANDLER

#include "Global.h"
#include "JobHandler.h"

#include "Controller.h"

class ForwardHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;
protected:
	/* A map of output data routes */
	smap<pair<string, size_t>> routes;
public:
	ForwardHandler(smap<pair<string, size_t>>& routes);
	virtual ~ForwardHandler();

	virtual void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif