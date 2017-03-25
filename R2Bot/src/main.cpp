#include "Global.h"

#include <vector>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <cmath>
#include <chrono>

#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif
#	define _WINSOCK_DEPRECATED_NO_WARNINGS
#	include <WinSock2.h>
#	include <Windows.h>
void initializeWSA() {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
}
#endif

#include "Controller.h"
#include "Job.h"
#include "JobHandler.h"
#include "Sensor.h"
#include "SensorData.h"

#include "Controller/UDPClientController.h"
#include "Controller/MotorController.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/R2Server.h"
#include "Sensor/UDPServerSensor.h"

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
	smap<ptr<Sensor>> sensors;
	sensors["udp server"] = std::make_shared<UDPServerSensor>("0.0.0.0", 9000);
	sensors["r2 server"] = std::make_shared<R2Server>(18080);
	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
	smap<ptr<Controller>> controllers;
	controllers["udp pi"] = std::make_shared<UDPClientController>("127.0.0.1", 9010);
	controllers["motor"] = std::make_shared<MotorController>("//./COM6", 9600);
	return controllers;
}

/** Initialize a list of jobs */
deque<Job> initializeJobs(smap<string>& args) {
	deque<Job> jobs;
	jobs.push_back(Job("manual-inputs"));
	return jobs;
}

/** Initialize background jobs */
deque<JobHandler> initializeBackgroundJobs(smap<string>& args) {
	deque<JobHandler> jobs;
	return jobs;
}

int main(int argc, char *argv[]) {
	smap<string> args = parseArguments(argc, argv);

	/** Initialization */
#ifdef _WIN32
	initializeWSA();
#endif
	smap<ptr<Sensor>> sensors = initializeSensors(args);
	smap<ptr<Controller>> controllers = initializeControllers(args);
	deque<Job> jobQueue = initializeJobs(args);
	ptr<JobHandler> currentJob;
	deque<JobHandler> bgJobs = initializeBackgroundJobs(args);

	// Data forwarding handler
	smap<ptr<Controller>> routes;
	routes["motor"] = controllers["udp pi"];
	routes["pi"] = controllers["udp pi"];
	ForwardHandler forwardHandler(routes);

	/** Main execution loop */
	while (1) {
#ifdef DEBUG_PRINTS
		printf("Loop Start\n");
#endif

		// Collect data from sensors
#ifdef DEBUG_PRINTS
		printf("Sensors\n");
#endif
		smap<ptr<SensorData>> data;
		for (auto itr : sensors) {
			ptr<Sensor> sensor = itr.second;
			sensor->getData(data);
		}

		// Execute current jobs
		if (!currentJob) {
			// Get the next job in the queue
			if (jobQueue.size() > 0) {
				Job nextJob = jobQueue.front();
				jobQueue.pop_front();
				currentJob = JobHandler::GetJobHandler(nextJob.getHandler());
			}
		}

		// Run the current job
		smap<string> outputs;
		if (currentJob) {
#ifdef DEBUG_PRINTS
			printf("Current job\n");
#endif
			currentJob->execute(jobQueue, data, outputs);
		}

		// Run background jobs
		for (auto itr : bgJobs) {
			itr.execute(jobQueue, data, outputs);
		}

		// Send output data to controllers
#ifdef DEBUG_PRINTS
		printf("Controllers\n");
#endif
		auto itr = outputs.begin();
		while (itr != outputs.end()) {
			auto controller = controllers.find(itr->first);
			if (controller != controllers.end()) {
				controller->second->sendData(itr->second);
				// Erase from the map
				itr = outputs.erase(itr);
			}
			else {
				itr++;
			}
		}

		// Forward any remaining outputs
#ifdef DEBUG_PRINTS
		printf("Forward\n");
#endif
		forwardHandler.execute(jobQueue, data, outputs);

		// Sleep
		Sleep(100);
	}
}
