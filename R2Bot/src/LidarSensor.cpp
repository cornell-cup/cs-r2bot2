#include "Sensor/LidarSensor.h"
#include "Data/LidarData.h"
#include <iostream>
#include <algorithm>

using namespace rp::standalone::rplidar;


LidarSensor::LidarSensor(string port, int baudrate) : Sensor("Lidar Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)), dataMutex() {
	printf("Lidar Sensor connected to port %s\n", port.c_str());

	bg_thread = std::thread(&LidarSensor::bg_scan, this);
	bg_thread.detach();

}
void LidarSensor::bg_scan() {
	u_result result;

	while (!bg_done) {
		rplidar_response_measurement_node_t scan_nodes[NODE_COUNT];
		size_t node_count = NODE_COUNT;

		result = driver->grabScanData(scan_nodes, node_count);

		if (IS_OK(result) || result == RESULT_OPERATION_TIMEOUT) {
			driver->ascendScanData(scan_nodes, node_count);

			std::lock_guard<std::mutex> lock(dataMutex);

			angles.clear();
			distances.clear();
			for (int i = 0; i < node_count; i++) {
				double angle = (scan_nodes[i].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0;
				double distance = scan_nodes[i].distance_q2 / 4000.0f;
				int quality = scan_nodes[i].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;

				if (quality >= 10) {
					angles.push_back(angle);
					distances.push_back(distance);
				}
			}
		}
	}
}

LidarSensor::~LidarSensor() {
	bg_done = true;
	RPlidarDriver::DisposeDriver(driver);
}

bool LidarSensor::ping() {
	return conn->isConnected() == 1;
}

void LidarSensor::fillData(SensorData& sensorData) {
	ptr<LidarData> ldata = std::make_shared<LidarData>();
	std::lock_guard<std::mutex> lock(dataMutex);

	unsigned int l = min(angles.size(), distances.size());
	for (int i = 0; i < l; i++) {
		ldata->angles.push_back(angles[i]);
		ldata->distances.push_back(distances[i]);
	}

	sensorData["LIDAR"] = ldata;
}