#ifndef _R2BOT_GLOBAL
#define _R2BOT_GLOBAL

#include "Config.h"

#include <deque>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

using std::deque;
using std::queue;
using std::pair;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

/** Pointer */
template<typename T> using ptr = shared_ptr<T>;
/** String => Class Pointer Map */
template<typename T> using smap = unordered_map<string, T>;
/** Sensor Data map<string, pair<ptr<void>, size_t>> */
typedef smap<ptr<void>> SensorData;
/** Controller Data map<string, pair<ptr<void>, size_t>> */
typedef smap<ptr<void>> ControllerData;

#ifndef _WIN32
#	define Sleep(x) usleep(x*1000)
#endif

/** Parse arguments as {--boolean | --key value} pairs */
inline smap<string> parseArguments(int argc, char ** argv) {
	smap<string> args;
	string key;
	for (int i = 0; i < argc; i++) {
		string part(argv[i]);
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
	static string DEVICE_NAME;
};

#endif