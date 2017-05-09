#include "JobHandler/SoundHandler.h"
#include "R2Protocol.hpp"

bool SoundHandler::registered = JobHandler::RegisterJobHandler("sound", [](string command) {
	return (ptr<JobHandler>) std::make_shared<SoundHandler>();
});

SoundHandler::SoundHandler() : JobHandler() {
}

SoundHandler::~SoundHandler() {
}

void SoundHandler::execute(deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	auto result = data.find("SOUND");
	if (result != data.end()) {
		outputs["SOUND"] = result->second;
	}
}