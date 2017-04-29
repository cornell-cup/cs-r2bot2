#include "Global.h"

#include <vector>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <cmath>
#include <chrono>

#include "Controller.h"
#include "Job.h"
#include "JobHandler.h"
#include "Sensor.h"

#include "Controller/UDPClientController.h"
#include "Controller/MotorController.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/R2Server.h"
#include "Sensor/UDPServerSensor.h"
#include "Sensor/UltrasoundSensor.h"

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

  return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
  smap<ptr<Controller>> controllers;
  if (!(args["motor-com-port"].empty())) {
    controllers["MOTOR"] = std::make_shared<MotorController>(args["motor-com-port"], 9600);
  }
  else {
    std::cout << "No motor ports specified." << std::endl;
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
  jobs.push_back(Job("safety"));
  jobs.push_back(Job("forward"));
  return jobs;
}

int main(int argc, char *argv[]) {

  smap<string> args = parseArguments(argc, argv);
  /** Initialization */
  smap<ptr<Sensor>> sensors = initializeSensors(args);
  smap<ptr<Controller>> controllers = initializeControllers(args);
  deque<Job> jobQueue = initializeJobs(args);
  ptr<JobHandler> currentJob;
  deque<JobHandler> bgJobs = initializeBackgroundJobs(args);

  // Data forwarding handler
  smap<ptr<Controller>> routes;
  routes["PICAMERA"] = controllers["UDP NUC"];
  ForwardHandler forwardHandler(routes);

  //maintainUsers(); //USER DATABASE STUFF

  //TODO: method arg should be sql command stored in a char

  /** Main execution loop */
  while (1) {
    //maintainTools();
    // Collect data from sensors
    SensorData data;
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
        controller->second->sendData(itr->second);
      }
      else {
        itr++;
      }
    }

    // Sleep
    Sleep(20);
  }
}
