#include "Global.h"

#include "Controller.h"
#include "Job.h"
#include "JobHandler.h"
#include "Sensor.h"
#include "SensorData.h"

#include "Controller/UDPClientController.h"
#include "JobHandler/ForwardHandler.h"
#include "Sensor/UDPServerSensor.h"

#include <vector>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <cmath>
#include <chrono>

#ifdef _WIN32
#	define _WINSOCK_DEPRECATED_NO_WARNINGS
#	include <WinSock2.h>
#	include <Windows.h>
void initializeWSA() {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
}
#endif

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
	smap<ptr<Sensor>> sensors;
	sensors["udp server"] = std::make_shared<UDPServerSensor>("127.0.0.1", 9000);
	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
	smap<ptr<Controller>> controllers;
	controllers["udp pi"] = std::make_shared<UDPClientController>("127.0.0.1", 9010);
	return controllers;
}

/** Initialize a list of jobs */
deque<Job> initializeJobs(smap<string>& args) {
	deque<Job> jobs;
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
	ptr<JobHandler> currentJob(0);
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
#ifdef DEBUG_PRINTS
		printf("Current jobs\n");
#endif
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
		Sleep(10);
	}
}
