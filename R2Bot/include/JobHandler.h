#ifndef _R2BOT_JOB_HANDLER
#define _R2BOT_JOB_HANDLER

#include "Global.h"

#include "Controller.h"
#include "Job.h"
#include "Sensor.h"

#include <memory>

class JobHandler;

typedef ptr<JobHandler>(*JobHandlerParser)(string);

class JobHandler {
private:
	/** Register this job handler */
	static bool registered;
protected:
	/** Types of job handlers and pointers to parsing function */
	static smap<JobHandlerParser>& JobHandlers();
public:
	/** Register this job handler for running jobs */
	static bool RegisterJobHandler(string handlerName, JobHandlerParser parser);

	/** Get a job handler by command */
	static ptr<JobHandler> GetJobHandler(string command);

	JobHandler();
	virtual ~JobHandler();

	/** Runs the job handler's actions */
	virtual void execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs);
};

#endif