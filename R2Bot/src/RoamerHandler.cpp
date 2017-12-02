#include JobHandler/RoamerHandler.h


bool RoamerHandler::registered = JobHandler::RegisterJobHandler("roamer", [](string command) {
	return (ptr<JobHandler>) std::make_shared<PathPlanningHandler>();
});

void PathPlanningHandler::execute(deque<Job>& jobs, SensorData & data, ControllerData & outputs)
{
	auto usInfo = data.find("ULTRASOUNDF");
	if (usInfo != data.end()) {
		ptr<UltrasoundData> usptr = std::static_pointer_cast<UltrasoundData>(usInfo->second);
		vector<float> usData = usptr->distance;
		for(int i = 0; i < usData.size(); i++){
			if(usData.at(i) != 0.f){
				ptr<MotorData> output = std::make_shared<MotorData>();
				if(usData.at(i) < 25.f){
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
