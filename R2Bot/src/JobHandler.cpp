#include "JobHandler.h"

#include <deque>
#include <string>

using std::string;

smap<JobHandlerParser>& JobHandler::JobHandlers() {
	static smap<JobHandlerParser> handlers;
	return handlers;
}

bool JobHandler::registered = JobHandler::RegisterJobHandler("default", [](string command) {
	return std::make_shared<JobHandler>();
});

bool JobHandler::RegisterJobHandler(string handlerName, JobHandlerParser parser) {
	JobHandler::JobHandlers()[handlerName] = parser;
	return true;
}

ptr<JobHandler> JobHandler::GetJobHandler(string command) {
	auto handler = JobHandler::JobHandlers().find(command);
	if (handler != JobHandler::JobHandlers().end()) {
		return handler->second(command);
	}
	else {
		return std::make_shared<JobHandler>();
	}
}

JobHandler::JobHandler() {
}

JobHandler::~JobHandler() {
}

void JobHandler::execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
}