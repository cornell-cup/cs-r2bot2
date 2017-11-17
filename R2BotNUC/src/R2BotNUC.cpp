#include "Global.h"

#include <chrono>
#include <cmath>
#include <deque>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using std::deque;
using std::string;
using std::vector;

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

#include "Controller/UDPClientController.h"
#include "Controller/MotorController.h"
#include "Controller/HeadFlapController.h"
#include "Controller/SoundController.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/R2Server.h"
#include "JobHandler/PowerHandler.h"
#include "JobHandler/SoundHandler.h"
#include "JobHandler/HeadHandler.h"
#include "JobHandler/HeadFlapHandler.h"
#include "JobHandler/SafetyHandler.h"
#include "Sensor/UDPServerSensor.h"
#include "Sensor/UltrasoundSensor.h"
#include "Sensor/DrawerSensor.h"
#include "Sensor/HeadSensor.h"
#include "Sensor/RFIDSensor.h"
//#include "Sensor/LidarSensor.h"
#include "Sensor/IMUSensor.h"
#include "Data/MotorData.h"
#include "Data/UltrasoundData.h"
#include "Data/DrawerData.h"

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
	smap<ptr<Sensor>> sensors;
	if (args.find("udp-server-ip") != args.end() && args.find("udp-server-port") != args.end()) {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>(args["udp-server-ip"], atoi(args["udp-server-port"].c_str()));
	}
	else {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>("0.0.0.0", 9000);
	}
	if (args.find("ultrasound-com-port") != args.end() && args.find("ultrasound-com-port2") != args.end()) {
		sensors["ULTRASOUND"] = std::make_shared<UltrasoundSensor>(args["ultrasound-com-port"], B9600, args["ultrasound-com-port2"], B9600);
	}
	else {
		std::cout << "No ultrasound port specified." << std::endl;
	}

	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
	smap<ptr<Controller>> controllers;
	if (args.find("udp-pi-ip") != args.end() && args.find("udp-pi-port") != args.end()) {
		controllers["UDP PI"] = std::make_shared<UDPClientController>("UDP PI", args["udp-pi-ip"], atoi(args["udp-pi-port"].c_str()));
	}
	else {
		std::cout << "No UDP Pi ip or port specified." << std::endl;
	}
	if (args.find("head-flap-serial-port") != args.end()) {
		controllers["R2 HEAD FLAP"] = std::make_shared<HeadFlapController>(args["head-flap-serial-port"].c_str(), 9600);
	}
	else {
		//controllers["R2 HEAD FLAP"] = std::make_shared<HeadFlapController>("COM4", 9600);
		std::cout << "No head flap serial port specified." << std::endl;
	}

	controllers["R2 SOUND"] = std::make_shared<SoundController>();

	return controllers;
}

/** Initialize a list of jobs */
deque<Job> initializeJobs(smap<string>& args) {
	deque<Job> jobs;
	jobs.push_back(Job("manual-inputs"));
	jobs.push_back(Job("path-planning"));
	return jobs;
}

/** Initialize background jobs */
deque<ptr<JobHandler>> initializeBackgroundJobs(smap<string>& args, smap<ptr<Sensor>>& sensors, smap<ptr<Controller>>& controllers) {
	deque<ptr<JobHandler>> jobs;
	if (args.find("disable-safety") != args.end()) {
		printf("WARNING: DISABLING MOTOR SAFETY\n");
	}
	else {
		jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<SafetyHandler>()));
	}
	// Data forwarding handler
	smap<std::pair<string, size_t>> routes;
	if (controllers.find("UDP PI") != controllers.end()) {
		routes["MOTOR"] = std::make_pair("UDP PI", sizeof(MotorData));
		routes["DRAWER"] = std::make_pair("UDP PI", sizeof(DrawerData));
		routes["ULTRASOUND"] = std::make_pair("UDP PI", sizeof(UltrasoundData));
		routes["PI"] = std::make_pair("UDP PI", 0);
	}
	jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<ForwardHandler>(routes)));
	//jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<R2Server>(18080)));
	ptr<R2Server> server = std::make_shared<R2Server>(18080);
	jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<SoundHandler>()));
	jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<HeadHandler>()));
	jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<HeadFlapHandler>()));
	jobs.push_back(std::static_pointer_cast<JobHandler>(server));
	sensors["R2 SERVER"] = std::static_pointer_cast<Sensor>(server);
	if (args.find("power-serial-port") != args.end()) {
		jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<PowerHandler>(args["power-serial-port"].c_str(), 9600)));
	}
	else {
		std::cout << "No power serial port specified." << std::endl;
	}
	return jobs;
}

int main(int argc, char *argv[]) {
	R2Bot::DEVICE_NAME = "NUC";

	smap<string> args = parseArguments(argc, argv);
	/** Initialization */
#ifdef _WIN32
	initializeWSA();
#endif
	smap<ptr<Sensor>> sensors = initializeSensors(args);
	smap<ptr<Controller>> controllers = initializeControllers(args);
	deque<Job> jobQueue = initializeJobs(args);
	ptr<JobHandler> currentJob;
	deque<ptr<JobHandler>> bgJobs = initializeBackgroundJobs(args, sensors, controllers);

	//TODO: method arg should be sql command stored in a char

	/** Main execution loop */
	while (1) {
		// Collect data from sensors
		SensorData data;
		for (auto itr : sensors) {
			ptr<Sensor> sensor = itr.second;
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
		ControllerData outputs;
		if (currentJob) {
			currentJob->execute(jobQueue, data, outputs);
		}

		// Run background jobs
		for (auto itr : bgJobs) {
			(*itr).execute(jobQueue, data, outputs);
		}

		// Send output data to controllers
		for (auto itr : controllers) {
			ptr<Controller> controller = itr.second;
			controller->sendData(outputs);
		}

		// Sleep
		Sleep(20);
	}
	
}
