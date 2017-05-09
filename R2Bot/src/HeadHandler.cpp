#include "JobHandler/HeadHandler.h"
#include "R2Protocol.hpp"
#include "Data/HeadData.h"
#include "JobHandler/R2Server.h"

string homeInput;
bool HeadHandler::registered = JobHandler::RegisterJobHandler("head", [](string command) {
	return (ptr<JobHandler>) std::make_shared<HeadHandler>();
});

HeadHandler::HeadHandler() : JobHandler() {
}

HeadHandler::~HeadHandler() {
}

void HeadHandler::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	auto result = data.find("HEAD");
	if (result != data.end()) {
		outputs["HEAD"] = result->second;
	}
}