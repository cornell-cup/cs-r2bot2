#include "Controller/SoundController.h"
#ifdef _WIN32
#	include <Windows.h>
#else
#	include <unistd.h>
#	include <stdlib.h>
#endif

#include "R2Protocol.hpp"
#include <cstring>
#include <thread>
#include <iostream>

SoundController::SoundController() : Controller("Sound Controller") {
	printf("Sound controller connected\n");
}

SoundController::~SoundController() {
}

bool SoundController::ping() {
	return true;
}

void SoundController::sendData(ControllerData& data) {
	auto result = data.find("SOUND");
	if (result != data.end()) {
		ptr<string> s = std::static_pointer_cast<string>(result->second);
		string path = "../R2Bot/templates/sounds/" + *s;
		std::cout << "Sound: " + path << std::endl;
#ifdef _WIN32
		std::wstring wpath = std::wstring(path.begin(), path.end());
		PlaySound(wpath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
#else
		std::thread sound([data]() {
			execlp("/usr/bin/aplay", " ", path.c_str(), NULL);		//Execute file: file, arguments (1 or more strings followed by NULL)
			_exit(0);
			printf("Playing sound: %s\n", s);
			}
		);
		sound.detach();
#endif	
	}
}