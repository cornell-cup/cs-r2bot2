#include "Controller/SoundController.h"
#include "Data/SoundData.h"

#ifdef _WIN32
#	include <Windows.h>
#endif

SoundController::SoundController() : Controller("Sound Controller") {
#ifdef _WIN32
	printf("Sound controller connected\n");
#else
	printf("Sound controller unsupported\n");
#endif
}

SoundController::~SoundController() {
}

bool SoundController::ping() {
	return true;
}

void SoundController::sendData(ControllerData& data) {
	auto result = data.find("SOUND");
	if (result != data.end()) {
		ptr<SoundData> s = std::static_pointer_cast<SoundData>(result->second);
		printf("Playing sound: %s\n", s->file.c_str());
#ifdef _WIN32
		PlaySound(s->file.c_str(), NULL, SND_FILENAME | SND_ASYNC);
#endif
	}
}