#include "JobHandler/ForwardHandler.h"
#include "R2Protocol.hpp"
#include "SensorData/ForwardSensorData.h"

bool ForwardHandler::registered = JobHandler::RegisterJobHandler("forward", [](string command) {
	smap<ptr<Controller>> routes;
	return (ptr<JobHandler>) std::make_shared<ForwardHandler>(routes);
});

ForwardHandler::ForwardHandler(smap<ptr<Controller>>& routes): JobHandler(), routes(routes) {
}

ForwardHandler::~ForwardHandler() {
}

void ForwardHandler::execute(deque<Job>& jobs, smap<ptr<SensorData>>& data, smap<string>& outputs) {
	// Send forwarded inputs
	auto forward = data.find("forward");
	auto packets = std::dynamic_pointer_cast<ForwardSensorData>(forward->second);
	for (auto itr : packets->forwardData) {
		auto route = routes.find(itr.first);
		if (route != routes.end()) {
			vector<uint8_t> output;
			R2Protocol::encode(itr.second, output);
			route->second->sendData(string(output.begin(), output.end()));
		}
	}
	// Send remaining outputs
	for (auto itr : outputs) {
		auto route = routes.find(itr.first);
		if (route != routes.end()) {
			route->second->sendData(itr.second);
		}
	}
}