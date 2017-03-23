#include "ManualInputs.h"
#include "UDPSocketServer.h"
#include "R2Protocol.hpp"
#include "Global.h"
#include "Controller.h"
#include "Job.h"
#include "JobHandler.h"
#include "Sensor.h"
#include "R2Server.h"

#include <vector>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <cmath>
#include <chrono>

ManualInputs *manualInputsHandler;

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string> args) {
	smap<ptr<Sensor>> sensors;
	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string> args) {
	smap<ptr<Controller>> controllers;
	return controllers;
}

/** Initialize job handlers */
smap<ptr<JobHandler>> initializeJobHandlers(smap<string> args) {
	smap<ptr<JobHandler>> handlers;
	return handlers;
}

/** Initialize a list of jobs */
smap<ptr<Job>> initializeJobs(smap<string> args) {
	smap<ptr<Job>> jobs;
	return jobs;
}

/** Parse arguments as {--boolean | --key value} pairs */
smap<string> parseArguments(int argc, char ** argv) {
	smap<string> args;
	string key;
	for (int i = 0; i < argc; i++) {
		string part(argv[i]);
		if (part.substr(0, 2) == "--") {
			if (key.length() > 0) { // Argument is present
				args[key] = "";
			}
			key = part;
		}
		else if (key.length() > 0) { // Found a value for a key
			args[key] = part;
			key = "";
		}
	}
	return args;
}

/** Initializes the manual inputs handler by establishing the UDP socket connection to receive data */
void initializeManualInputsHandler(std::string host, int port) {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	manualInputsHandler = new ManualInputs(host, port);
	(*manualInputsHandler).receiveManualInputs();
}

/** Utility function for splitting a string based on a delimitting character */
void split(const std::string& s, char c, std::vector<std::string>& v) {
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);

	while (j != std::string::npos) {
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);
		if (j == std::string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

/** Pulls data from the manual inputs queue and performs the associated action */
void processManualData() {
	while (TRUE) {
		std::queue<std::string> *taskQueue = (*manualInputsHandler).getCommands();
		if ((*taskQueue).size() > 0) {
			std::string manualCommand = (*taskQueue).front();
			(*taskQueue).pop();
			if (manualCommand.find("XBOX") == 0) {
				std::vector<std::string> v;
				std::string s = manualCommand.substr(4, 100);
				split(s, ' ', v);
				for (int i = 0; i < v.size(); ++i) {
					std::string tmp = v[i];
					char number[1024];
					strcpy_s(number, tmp.c_str());
				}
				int left = std::stoi(v[0]);
				int right = std::stoi(v[1]);
				std::cout << "Left motor speed: " << left << "\n";
				std::cout << "Right motor speed: " << right << "\n";
				std::vector<uint8_t> data;
				uint8_t commandsL[]= "S{L}";
				data.push_back(commandsL[0]);
				data.push_back(commandsL[1]);
				std::string leftMotor = std::to_string(left);
				uint8_t commandsR[] = "{R}";
				data.push_back(commandsR[0]);
				std::string rightMotor = std::to_string(right);
				for (int track = 0; track < data.size(); ++track) {
					std::cout << data[track];
				}
				std::vector<uint8_t> output;
				R2Protocol::Packet packet = {
					"NUC", //Source of information is NUC
					"MOTORS", //Trying to send information to motors
					"MTs", //Made up transaction id
					data, //vector containing the data we want to transmit
					"", //checksum, left blank, will be calculated in encode method
				};
				R2Protocol::encode(packet, output);
			}
		}
	}
}

int main(int argc, char *argv[]) {
	//smap<string> args = parseArguments(argc, argv);

	//server();
	///** Initialization */
	//smap<ptr<Sensor>> sensors = initializeSensors(args);
	//smap<ptr<Controller>> controllers = initializeControllers(args);
	//smap<ptr<JobHandler>> handlers = initializeJobHandlers(args);
	//smap<ptr<Job>> jobs = initializeJobs(args);
	initializeManualInputsHandler("10.211.55.3", 9020);
	processManualData();

	///** Main execution loop */
	//while (1) {
	//	// Collect data from sensors
	//	unordered_map<string, void*> data;
	//	for (auto itr : sensors) {
	//		ptr<Sensor> sensor = itr.second;
	//		sensor->getData(data);
	//	}

	//	// Execute jobs
	//	smap<ptr<Job>> newJobs;
	//	for (auto itr : jobs) {
	//		ptr<Job> job = itr.second;
	//		ptr<JobHandler> handler = handlers[job->getHandler()];
	//		smap<ptr<Job>> addedJobs = handler->execute(data);
	//		// Add new jobs
	//		for (auto additr : addedJobs) {
	//			newJobs[additr.first] = additr.second;
	//		}
	//	}

	//	// Add new jobs, overwriting existing ones
	//	for (auto itr : newJobs) {
	//		jobs[itr.first] = itr.second;
	//	}
	//}
}