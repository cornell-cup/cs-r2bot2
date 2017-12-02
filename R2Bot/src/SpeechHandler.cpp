#include "JobHandler/SpeechHandler.h"
#include "R2Protocol.hpp"

#include <deque>
#include <string>

using std::string;

bool SpeechHandler::registered = JobHandler::RegisterJobHandler("speech", [](string command) {
	return (ptr<JobHandler>) std::make_shared<SpeechHandler>();
});

SpeechHandler::SpeechHandler() : JobHandler() {
}

SpeechHandler::~SpeechHandler() {
}

void SpeechHandler::execute(std::deque<Job>& jobs, SensorData& data, ControllerData& outputs) {
	auto result = data.find("SPEECH");
	if (result != data.end()) {
		ptr<string> s = std::static_pointer_cast<string>(result->second);
		if((*s).compare("hello are too") == 0)
			outputs["SOUND"] = std::make_shared<string>("cheerful.wav");
		else if ((*s).compare("goodbye are too") == 0)
		{
			outputs["SOUND"] = std::make_shared<string>("sad1.wav");
		}
	}
}