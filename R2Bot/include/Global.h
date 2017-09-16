#ifndef _R2BOT_GLOBAL
#define _R2BOT_GLOBAL

#include "Config.h"

#include <deque>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

/** Pointer */
template<typename T> using ptr = std::shared_ptr<T>;
/** String => Class Pointer Map */
template<typename T> using smap = std::unordered_map<std::string, T>;
/** Sensor Data map<string, pair<ptr<void>, size_t>> */
typedef smap<ptr<void>> SensorData;
/** Controller Data map<string, pair<ptr<void>, size_t>> */
typedef smap<ptr<void>> ControllerData;

#ifndef _WIN32
#	define Sleep(x) usleep(x*1000)
#endif

/** Parse arguments as {--boolean | --key value} pairs */
inline smap<std::string> parseArguments(int argc, char ** argv) {
	smap<std::string> args;
	std::string key;
	for (int i = 0; i < argc; i++) {
		std::string part(argv[i]);
		if (part.substr(0, 2) == "--") {
			if (key.length() > 0) { // Argument is present
				args[key] = "";
			}
			key = part.substr(2);
		}
		else if (key.length() > 0) { // Found a value for a key
			args[key] = part;
			key = "";
		}
	}
	return args;
}

class R2Bot {
public:
	static std::string DEVICE_NAME;

	static const int LEVEL_FATAL = 0;
	static const int LEVEL_ERROR = 10;
	static const int LEVEL_WARN = 20;
	static const int LEVEL_INFO = 30;
	static const int LEVEL_DEBUG = 40;
	static const int LEVEL_TRACE = 50;

	static int LOG_LEVEL;
	static std::string LOG_FILTER;

	static void log(int level, std::string topic, std::string message);
	static void setLogFilter(std::string topic);
	static void setLogLevel(int level);
};

#endif