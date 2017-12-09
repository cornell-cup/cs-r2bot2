#include "Sensor/UltrasoundSensor.h"
#include "Data/UltrasoundData.h"

#include <iostream>
#include <string>

using std::string;
UltrasoundSensor::UltrasoundSensor(string port, int baudrate, string port2, int baudrate2) : Sensor("Ultrasound Sensor"), connF(std::make_shared<SerialPort>(port, baudrate)), connB(std::make_shared<SerialPort>(port2, baudrate2)), dataMutex(), readData() {
    printf("Ultrasound connected to port %s\n", port.c_str());
	printf("Ultrasound connected to port %s\n", port2.c_str());
}

UltrasoundSensor::~UltrasoundSensor() {
}

bool UltrasoundSensor::ping() {
	return connF->isConnected() == 1 && connB->isConnected();
}

std::vector<int> find_nums(const char *start, const char *end, int num) {
    std::vector<int> ret(num);

    int i = 0;
    int prev = 0;
    int inti = 0;
    int sum = 0;
    while(start+i != end) {
        if(start[i] == ' ' || start[i] == '\n') {
            if (i != prev) {
                ret[inti] = sum;
                inti++;
                sum = 0;
            }
            prev = i+1;
        } else if (start[i] >= '0' && start[i] <= '9') {
            sum = 10*sum + (start[i] - '0');
        }

        i++;
        if (inti >= num) {
            break;
        }
    }

    return ret;
}

void UltrasoundSensor::fillData(SensorData& sensorData) {
	if (connF->isConnected()) {
		char data[512];
        int bytesRead = connF->read(data, 512);
        if (bytesRead <= 0) {
		    return;
		}
        std::string new_string(data, bytesRead);
        readData += new_string;
        // printf("Data: %s", readData.data());        
		std::lock_guard<std::mutex> lock(dataMutex);

		ptr<UltrasoundData> udata = std::make_shared<UltrasoundData>();
		int index1 = -1;
		int index2 = -1;
		for (int x = readData.length()-1; x >= 0; x--) {
			if (readData[x] == '\n') {
				index1 = x;
				for (int y = x - 1; y >= 0; y--) {
					if (readData[y] == '\n') {
						index2 = y;
						break;
					}
				}
				break;
			}
		}
        index2 = 0;
        //printf("Data: %s\n", readData.data());
        //printf("Index 1: %d \n", index1);
        //printf("Index 2: %d \n", index2);
		// float num = 0;
		if (index1 != -1 && index2 != -1 && index1 != index2) {
			std::vector<float> u;
            std::vector<int> nums = find_nums(readData.data()+index2+1, readData.data()+index1, 5);
            for(size_t i = 0; i < nums.size(); i++) {
                float val = ((float) nums[i]) * (5.f / 1024.f) * (1.f / 0.0098f);
                u.push_back(val);
                //printf("Value being pushed onto vector: %f \n", val);
            }
			for(int t = nums.size(); t < 4; t++){
				u.push_back(0.f);
			}
			udata->distance = u;
			sensorData["ULTRASOUNDF"] = udata;
            readData = readData.substr(index1);
		}

		// Decode the incoming data
		/*
		R2Protocol::Packet params;
		std::vector<uint8_t> input(data, data + bytesRead);
		int32_t read;
		ptr<UltrasoundData> udata = std::make_shared<UltrasoundData>();

		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if (params.source == "ULTRASOUND") {
				std::vector<float> u;
				string t(params.data.begin(), params.data.end());
				t = t + ",";
				string delimiter = ",";
				size_t pos = 0;
				std::string token;
				while ((pos = t.find(delimiter)) != std::string::npos) {
					token = t.substr(0, pos);
					std::cout << stof(token) << std::endl;
					u.push_back(stof(token));
					t.erase(0, pos + delimiter.length());
				}
				udata->distance = u;

				std::vector<uint8_t> newinput(input.begin() + read, input.end());
				newinput.swap(input);
				if (params.id == "F") {
					sensorData[params.source + "F"] = udata;
				}
				else if (params.id == "B") {
					sensorData[params.source + "B"] = udata;
				}
			}
		}
		*/
	}
	if (connB->isConnected()) {
		char data[256];
		int bytesRead = connB->read(data, 256);
		if (bytesRead <= 0) {
			return;
		}
		std::lock_guard<std::mutex> lock(dataMutex);
		// Decode the incoming data
		R2Protocol::Packet params;
		std::vector<uint8_t> input(data, data + bytesRead);
		ptr<UltrasoundData> udata = std::make_shared<UltrasoundData>();
        /*
		if ((read = R2Protocol::decode(input, params, 1)) >= 0) {
			if (params.source == "ULTRASOUND") {
				std::vector<float> u;
				string t(params.data.begin(), params.data.end());
				t = t + ",";
				string delimiter = ",";
				size_t pos = 0;
				std::string token;
				while ((pos = t.find(delimiter)) != std::string::npos) {
					token = t.substr(0, pos);
					u.push_back(stof(token));
					t.erase(0, pos + delimiter.length());
				}
				udata->distance = u;

				std::vector<uint8_t> newinput(input.begin() + read, input.end());
				newinput.swap(input);
				if (params.id == "F") {
					sensorData[params.source + "F"] = udata;
				}
				else if (params.id == "B") {
					sensorData[params.source + "B"] = udata;
				}
			}
		}
        */
	}
}
