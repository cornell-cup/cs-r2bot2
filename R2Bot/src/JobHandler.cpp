#include "JobHandler.h"

smap<JobHandlerParser> JobHandler::JobHandlers;

void JobHandler::RegisterJobHandler(string handlerName, JobHandlerParser parser) {
	JobHandler::JobHandlers[handlerName] = parser;
}

ptr<JobHandler> JobHandler::GetJobHandler(string command) {
	auto handler = JobHandler::JobHandlers.find(command);
	if (handler != JobHandler::JobHandlers.end()) {
		return handler->second(command);
	}
	else {
		return std::make_shared<JobHandler>();
	}
}

JobHandler::JobHandler()
{
}

JobHandler::~JobHandler()
{
}

void JobHandler::execute(deque<Job>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs)
{
}