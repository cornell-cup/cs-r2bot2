#include "JobHandler/ForwardHandler.h"
#include "R2Protocol.hpp"
#include "Data/ForwardData.h"

#include <memory>

using std::deque;
using std::pair;
using std::string;
using std::vector;

bool ForwardHandler::registered = JobHandler::RegisterJobHandler("forward", [](string command) {
	smap<pair<string, size_t>> routes;
	return (ptr<JobHandler>) std::make_shared<ForwardHandler>(routes);
});

ForwardHandler::ForwardHandler(smap<pair<string, size_t>>& routes): JobHandler(), routes(routes) {
}

ForwardHandler::~ForwardHandler() {
}

void ForwardHandler::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	ptr<ForwardData> forwardData;

	// Send forwarded inputs
	auto forward = data.find("FORWARD");
	if (forward != data.end()) {
		forwardData = std::static_pointer_cast<ForwardData>(forward->second);
	}
	else {
		forwardData = std::make_shared<ForwardData>();
	}

	// Send remaining outputs
	for (auto itr : outputs) {
		auto route = routes.find(itr.first);
		if (route != routes.end()) {
			string destination = route->second.first;
			size_t data_len = route->second.second;
			uint8_t * dataptr = (uint8_t *)(itr.second.get());
			vector<uint8_t> data(dataptr, dataptr + data_len);
			R2Protocol::Packet packet = { R2Bot::DEVICE_NAME, itr.first, "OUTPUT", data };
			forwardData->data[destination].push_back(packet);
		}
	}
	outputs["FORWARD"] = forwardData;
}