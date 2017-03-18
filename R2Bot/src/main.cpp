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

int main(int argc, char *argv[]) {
	smap<string> args = parseArguments(argc, argv);

	server();
	/** Initialization */
	smap<ptr<Sensor>> sensors = initializeSensors(args);
	smap<ptr<Controller>> controllers = initializeControllers(args);
	smap<ptr<JobHandler>> handlers = initializeJobHandlers(args);
	smap<ptr<Job>> jobs = initializeJobs(args);

	/** Main execution loop */
	while (1) {
		// Collect data from sensors
		unordered_map<string, void*> data;
		for (auto itr : sensors) {
			ptr<Sensor> sensor = itr.second;
			sensor->getData(data);
		}

		// Execute jobs
		smap<ptr<Job>> newJobs;
		for (auto itr : jobs) {
			ptr<Job> job = itr.second;
			ptr<JobHandler> handler = handlers[job->getHandler()];
			smap<ptr<Job>> addedJobs = handler->execute(data);
			// Add new jobs
			for (auto additr : addedJobs) {
				newJobs[additr.first] = additr.second;
			}
		}

		// Add new jobs, overwriting existing ones
		for (auto itr : newJobs) {
			jobs[itr.first] = itr.second;
		}
	}
}