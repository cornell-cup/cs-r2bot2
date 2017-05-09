#include "JobHandler/HeadFlapHandler.h"
#include "R2Protocol.hpp"

bool HeadFlapHandler::registered = JobHandler::RegisterJobHandler("head-flap", [](string command) {
	return (ptr<JobHandler>) std::make_shared<HeadFlapHandler>();
});

HeadFlapHandler::HeadFlapHandler() : JobHandler() {
}

HeadFlapHandler::~HeadFlapHandler() {
}

void HeadFlapHandler::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	auto result = data.find("FLAP");
	if (result != data.end()) {
		outputs["FLAP"] = result->second;
	}
}