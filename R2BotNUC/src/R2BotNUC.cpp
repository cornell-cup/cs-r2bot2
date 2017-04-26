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
#include "JobHandler/R2Users.h"
#include "JobHandler/R2Tools.h"

#include "Controller/UDPClientController.h"
#include "Controller/MotorController.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/R2Server.h"
#include "Sensor/UDPServerSensor.h"
#include "Sensor/UltrasoundSensor.h"
#include "Sensor/DrawerSensor.h"

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
	smap<ptr<Sensor>> sensors;
	if (!(args["udp-server-ip"].empty()) && !(args["udp-server-port"].empty())) {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>(args["udp-server-ip"], atoi(args["udp-server-port"].c_str()));
	}
	else {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>("0.0.0.0", 9000);
	}
	if (!(args["r2-server-port"].empty())) {
		sensors["R2 SERVER"] = std::make_shared<R2Server>(atoi(args["r2-server-port"].c_str()));
	}
	else {
		sensors["R2 SERVER"] = std::make_shared<R2Server>(18080);
	}
	if (!(args["ultrasound-serial-port"].empty())) {
		sensors["R2 ULTRASOUND"] = std::make_shared<UltrasoundSensor>(args["ultrasound-sensor-port"].c_str(), 9600);
	}
	else {
		std::cout << "No ultrasound serial port specified." << std::endl;
	}

	if (!(args["drawer-serial-port"].empty())) {
		sensors["R2 DRAWER"] = std::make_shared<DrawerSensor>(args["drawer-sensor-port"].c_str(), 9600);
	}
	else {
		std::cout << "No drawer serial port specified." << std::endl;
	}

	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
	smap<ptr<Controller>> controllers;
	if (!(args["udp-pi-ip"].empty()) && !(args["udp-pi-port"].empty())) {
		controllers["UDP PI"] = std::make_shared<UDPClientController>(args["udp-pi-ip"], atoi(args["udp-pi-port"].c_str()));
	}
	else {
		std::cout << "No UDP Pi ip or port specified." << std::endl;
	}

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
	routes["MOTOR"] = controllers["UDP PI"];
	routes["PI"] = controllers["UDP PI"];
	ForwardHandler forwardHandler(routes);

	//maintainUsers(); //USER DATABASE STUFF

	//TODO: method arg should be sql command stored in a char

	/** Main execution loop */
	while (1) {
		//maintainTools();
		// Collect data from sensors
		SensorData data;
		for (auto itr : sensors) {
			//printf("test 1\n");
			ptr<Sensor> sensor = itr.second;
			//printf("test 2\n");
			//std::cout << sensor->getName() << std::endl;
			sensor->fillData(data);
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
		smap<ptr<void>> outputs;
		if (currentJob) {
			currentJob->execute(jobQueue, data, outputs);
		}

		// Run background jobs
		for (auto itr : bgJobs) {
			itr.execute(jobQueue, data, outputs);
		}

		// Send output data to controllers
		auto itr = outputs.begin();
		while (itr != outputs.end()) {
			auto controller = controllers.find(itr->first);
			if (controller != controllers.end()) {
				smap<ptr<void>> m;
				m[itr->first] = itr->second;
				controller->second->sendData(m);
				// Erase from the map
				itr = outputs.erase(itr);
			}
			else {
				itr++;
			}
		}

		// Forward any remaining outputs
		forwardHandler.execute(jobQueue, data, outputs);
		// Sleep
		Sleep(100);
	}
}
