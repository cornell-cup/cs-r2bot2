#ifndef _R2BOT_MANUAL_INPUTS_HANDLER
#define _R2BOT_MANUAL_INPUTS_HANDLER

#include "Global.h"
#include "JobHandler.h"

class ManualInputsHandler : public JobHandler {
private:
	/** Register this job handler */
	static bool registered;

	/** Pad an integer into a string */
	string _pad(int i, int l);
protected:
public:
	ManualInputsHandler();
	virtual ~ManualInputsHandler();

	virtual void execute(deque<Job>& jobs, SensorData& data, smap<string>& outputs);
};

#endif