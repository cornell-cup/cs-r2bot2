#include "Global.h"

#include <deque>
#include <vector>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <string>

#include "Controller.h"
#include "Job.h"
#include "JobHandler.h"
#include "Sensor.h"

#include "Controller/UDPClientController.h"
#include "Controller/MotorController.h"
#include "JobHandler/ForwardHandler.h"
#include "JobHandler/SafetyHandler.h"
#include "JobHandler/R2Server.h"
#include "Sensor/UDPServerSensor.h"
#include "Sensor/UltrasoundSensor.h"

using std::string;
using std::vector;
using std::deque;

/** Initializes sensors */
smap<ptr<Sensor>> initializeSensors(smap<string>& args) {
  smap<ptr<Sensor>> sensors;
  if (!(args["udp-server-ip"].empty()) && !(args["udp-server-port"].empty())) {
    sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>(args["udp-server-ip"], atoi(args["udp-server-port"].c_str()));
  }
  else {
    sensors["UDP SERVER"] = std::make_shared<UDPServerSensor>("0.0.0.0", 9000);
  }

  if(!(args["ultrasound-com-port"].empty())){
    sensors["ULTRASOUND"] = std::make_shared<UltrasoundSensor>(args["ultrasound-com-port"], B9600, "", B9600);
  }
  else{
    std::cout << "No ultrasound ports specified." << std::endl;
  }

  return sensors;
}

/** Initializes controllers */
smap<ptr<Controller>> initializeControllers(smap<string>& args) {
  smap<ptr<Controller>> controllers;
  if (!(args["motor-com-port"].empty())) {
    controllers["MOTOR"] = std::make_shared<MotorController>(args["motor-com-port"], B9600);
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
deque<ptr<JobHandler>> initializeBackgroundJobs(smap<string>& args, smap<ptr<Sensor>> &sensors, smap<ptr<Controller>> &controllers) {
  deque<ptr<JobHandler>> jobs;
  if (!(args.find("disable-safety").empty())) {
		printf("WARNING: DISABLING MOTOR SAFETY\n");
	}
	else {
		jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<SafetyHandler>()));
	}
  //jobs.push_back(std::static_pointer_cast<JobHandler>(std::make_shared<SafetyHandler>()));
  ptr<R2Server> server = std::make_shared<R2Server>(18080);
  sensors["R2 SERVER"] = std::static_pointer_cast<Sensor>(server);
  jobs.push_back(std::static_pointer_cast<JobHandler>(server));
  return jobs;
}

int main(int argc, char *argv[]) {
  R2Bot::DEVICE_NAME = "PI";
  smap<string> args = parseArguments(argc, argv);
  /** Initialization */
  smap<ptr<Sensor>> sensors = initializeSensors(args);
  smap<ptr<Controller>> controllers = initializeControllers(args);
  deque<Job> jobQueue = initializeJobs(args);
  ptr<JobHandler> currentJob;
  deque<ptr<JobHandler>> bgJobs = initializeBackgroundJobs(args, sensors, controllers);

  // Data forwarding handler

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
