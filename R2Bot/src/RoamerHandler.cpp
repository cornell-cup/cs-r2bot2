#include "JobHandler/RoamerHandler.h"
#include "Data/MotorData.h"
#include <deque>
#include <string>
using std::string;

bool RoamerHandler::registered = JobHandler::RegisterJobHandler("roamer", [](string command) {
	return (ptr<JobHandler>) std::make_shared<RoamerHandler>();
});

void RoamerHandler::execute(std::deque<Job>& jobs, SensorData & data, ControllerData & outputs)
{
	auto usInfo = data.find("ULTRASOUNDF");
	if (usInfo != data.end()) {
		ptr<UltrasoundData> usptr = std::static_pointer_cast<UltrasoundData>(usInfo->second);
		std::vector<float> usData = usptr->distance;
		for(int i = 0; i < 8; i++){
            printf("Ultrasound: %f \n", usData[i]);
			if(usData[i] != 0.f){
                ptr<MotorData> output = std::make_shared<MotorData>();
				if(usData[i] < 25.f){
					output->leftMotor = 0;
					output->rightMotor = 0;
					outputs["MOTOR"] = output;
				}else{
					output->leftMotor = 150;
					output->rightMotor = 150;
					outputs["MOTOR"] = output;
				}
			}else{
				break;
			}
		}
	}
}
