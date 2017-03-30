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
	smap<ptr<Controller>> routes;
public:
	ForwardHandler(smap<ptr<Controller>>& routes);
	virtual ~ForwardHandler();

	virtual void execute(deque<Job>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs);
};

#endif