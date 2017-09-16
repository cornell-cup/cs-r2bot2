#include "Global.h"

#include <iostream>
#include <string>

std::string R2Bot::DEVICE_NAME = "";

int R2Bot::LOG_LEVEL = R2Bot::LEVEL_INFO;
std::string R2Bot::LOG_FILTER = "";

void R2Bot::log(int level, std::string topic, std::string message) {
	if (level <= R2Bot::LOG_LEVEL && (R2Bot::LOG_FILTER == "" || topic == R2Bot::LOG_FILTER)) {
		std::cout << "[" << topic << "]" << message << std::endl;
	}
}

void R2Bot::setLogFilter(std::string filter) {
	R2Bot::LOG_FILTER = filter;
}

void R2Bot::setLogLevel(int level) {
	R2Bot::LOG_LEVEL = level;
}