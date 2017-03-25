#include "JobHandler.h"

smap<JobHandlerParser> JobHandler::JobHandlers;

void JobHandler::RegisterJobHandler(string handlerName, JobHandlerParser parser) {
	JobHandler::JobHandlers[handlerName] = parser;
}

JobHandler::JobHandler()
{
}

JobHandler::~JobHandler()
{
}

void JobHandler::execute(smap<ptr<Job>>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs)
{
}