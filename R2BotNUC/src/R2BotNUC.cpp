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

#include "Controller/UDPClientController.h"
#include "Controller/MotorController.h"
#include "Controller/HeadFlapController.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/R2Server.h"
#include "Sensor/UDPServerSensor.h"
#include "Sensor/UltrasoundSensor.h"
#include "Sensor/DrawerSensor.h"
#include "Sensor/HeadSensor.h"
#include "Sensor/RFIDSensor.h"
#include "Sensor/IMUSensor.h"

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
	smap<ptr<Sensor>> sensors;
	if (args.find("udp-server-ip") != args.end() && args.find("udp-server-port") != args.end()) {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>(args["udp-server-ip"], atoi(args["udp-server-port"].c_str()));
	}
	else {
		sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>("0.0.0.0", 9000);
	}
	if (args.find("r2-server-port") != args.end()) {
		sensors["R2 SERVER"] = std::make_shared<R2Server>(atoi(args["r2-server-port"].c_str()));
	}
	else {
		sensors["R2 SERVER"] = std::make_shared<R2Server>(18080);
	}

	if (!(args["ultrasound-serial-port"].empty())) {
		sensors["R2 ULTRASOUND"] = std::make_shared<UltrasoundSensor>(args["ultrasound-serial-port"].c_str(), 9600);
	}
	else {
		std::cout << "No ultrasound serial port specified." << std::endl;
	}

	if (!(args["drawer-serial-port"].empty())) {
		sensors["R2 DRAWER"] = std::make_shared<DrawerSensor>(args["drawer-serial-port"].c_str(), 9600);
	}
	else {
		std::cout << "No drawer serial port specified." << std::endl;
	}

	if (!(args["head-serial-port"].empty())) {
		sensors["R2 HEAD"] = std::make_shared<HeadSensor>(args["head-serial-port"].c_str(), 9600);
	}
	else {
		std::cout << "No head serial port specified." << std::endl;
	}

	if (!(args["rfid-serial-port"].empty())) {
		sensors["R2 RFID"] = std::make_shared<RFIDSensor>(args["rfid-serial-port"].c_str(), 9600);
	}
	else {
		std::cout << "No RFID serial port specified." << std::endl;
	}
	if (!(args["lidar-serial-port"].empty())) {
		sensors["R2 LIDAR"] = std::make_shared<RFIDSensor>(args["lidar-serial-port"].c_str(), 9600);
	}
	else {
		std::cout << "No LIDAR serial port specified." << std::endl;
	}

	if (!(args["imu-serial-port"].empty())) {
		sensors["R2 IMU"] = std::make_shared<IMUSensor>(args["imu-serial-port"].c_str(), 9600);
	}
	else {
		std::cout << "No IMU serial port specified." << std::endl;
	}

	return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
	smap<ptr<Controller>> controllers;
	if (args.find("udp-pi-ip") != args.end() && args.find("udp-pi-port") != args.end()) {
		controllers["UDP PI"] = std::make_shared<UDPClientController>(args["udp-pi-ip"], atoi(args["udp-pi-port"].c_str()));
	}
	else {
		std::cout << "No UDP Pi ip or port specified." << std::endl;
	}
	if (!(args["head-flap-serial-port"].empty())) {
		controllers["R2 HEAD FLAP"] = std::make_shared<HeadFlapController>(args["head-flap-serial-port"].c_str(), 9600);
	}
	else {
		std::cout << "No head flap serial port specified." << std::endl;
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
deque<JobHandler> initializeBackgroundJobs(smap<string>& args, smap<ptr<Sensor>>& sensors, smap<ptr<Controller>>& controllers) {
	deque<JobHandler> jobs;

	// Data forwarding handler
	smap<ptr<Controller>> routes;
	if (controllers.find("UDP PI") != controllers.end()) {
		routes["MOTOR"] = controllers["UDP PI"];
		routes["DRAWER"] = controllers["UDP PI"];
		routes["ULTRASOUND"] = controllers["UDP PI"];
		routes["PI"] = controllers["UDP PI"];
	}
	jobs.push_back(ForwardHandler(routes));

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
	deque<JobHandler> bgJobs = initializeBackgroundJobs(args, sensors, controllers);

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
			itr.execute(jobQueue, data, outputs);
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
