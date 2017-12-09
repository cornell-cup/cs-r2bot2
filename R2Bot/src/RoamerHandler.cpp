#include "JobHandler/RoamerHandler.h"
#include "Data/MotorData.h"
#include "Data/GamepadData.h"
#include <deque>
#include <string>

using std::string;

bool RoamerHandler::registered = JobHandler::RegisterJobHandler("roamer", [](string command) {
	return (ptr<JobHandler>) std::make_shared<RoamerHandler>();
});

RoamerHandler::RoamerHandler() : JobHandler() {
}

RoamerHandler::~RoamerHandler() {
}

void RoamerHandler::execute(std::deque<Job>& jobs, SensorData &data, ControllerData &outputs)
{
    auto now = std::chrono::steady_clock::now();
    auto gmInfo = data.find("GAMEPAD");
    ptr<MotorData> output = std::make_shared<MotorData>();
    if (gmInfo != data.end()) {
        auto gamepaddata = std::static_pointer_cast<GamepadData>(gmInfo->second);
        if (gamepaddata->rb) {
            rb = gamepaddata->rb;
            lastActivity = now;
        }
        if (gamepaddata->lb) {
            lb = gamepaddata->lb;
            lastActivity = now;
        }

        auto diff = now - lastActivity;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() > 500) {
            lb = false;
            rb = false;
            output->leftMotor = 0;
            output->rightMotor = 0;
        }
    }
	auto usInfo = data.find("ULTRASOUNDF");
    output->leftMotor = 0;
    output->rightMotor = 0;
    if (usInfo != data.end() && gmInfo != data.end()) {
		ptr<UltrasoundData> usptr = std::static_pointer_cast<UltrasoundData>(usInfo->second);
		std::vector<float> usData = usptr->distance;
        for(int i = 0; i < 8; i++){
			if(usData[i] != 0.f){
				if (usData[i] < 25.f){
                    if (rb) {
    					output->leftMotor = 100;
    					output->rightMotor = -100;
                    }
                    else if (lb) {
    					output->leftMotor = -100;
    					output->rightMotor = 100;
                    }
                    else {
                        output->leftMotor = 0;
                        output->rightMotor = 0;
                    }
				}
                else if (rb || lb) {
					output->leftMotor = 200;
					output->rightMotor = 200;
				}
                else {
                    output->leftMotor = 0;
                    output->rightMotor = 0;
                }
                outputs["MOTOR"] = output;
			} else {
				break;
			}
		}
	}
}
